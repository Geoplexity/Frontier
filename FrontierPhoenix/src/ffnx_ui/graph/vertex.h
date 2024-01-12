#ifndef FFNX_UI_GRAPH_VERTEX
#define FFNX_UI_GRAPH_VERTEX

#include "vertex_positioning_engine.h"
#include <QGraphicsItem>
#include <iostream>

namespace ffnx::ui::graph {

    template <typename TGraph>
    class Vertex : public QGraphicsItem {
    public:
        enum {
            Type = UserType + 1
        };
        int type() const override {
            return Type;
        }

        typename TGraph::vertex_descriptor vertex_descriptor;

        std::weak_ptr<VertexPositioningEngine<TGraph>> vertex_positioning_engine;

        Vertex(
                std::weak_ptr<VertexPositioningEngine<TGraph>> vertex_positioning_engine,
                const typename TGraph::vertex_descriptor &vertex_descriptor) :
                    vertex_positioning_engine(vertex_positioning_engine),
                    vertex_descriptor(vertex_descriptor) {
            setFlag(ItemIsMovable);
            setFlag(ItemSendsGeometryChanges);
            setCacheMode(DeviceCoordinateCache);

            vertex_positioning_engine.lock()->add_vert_listener(vertex_descriptor, [this](){
                this->reposition();
            });
        }

        void reposition() {
            prepareGeometryChange();

            auto coord = vertex_positioning_engine.lock()->get_vertex_coordinate(vertex_descriptor);

            setPos(std::get<0>(coord), std::get<1>(coord));
        }

        QRectF boundingRect() const override {
            return QRectF(-10, -10, 20, 20);
        }

        QPainterPath shape() const override {
            QPainterPath path;
            path.addEllipse(-10, -10, 20, 20);
            return path;
        }

        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override {
            painter->setPen(QPen(Qt::black));
            painter->setBrush(Qt::white);
            painter->drawEllipse(-10, -10, 20, 20);
        }

        QVariant itemChange(GraphicsItemChange change, const QVariant &value) override {

            if (change == ItemPositionChange) {
                auto new_pos = value.value<QPointF>();

                if (scene()->mouseGrabberItem() == this) {
                    // update the vertex engine with the new requested coordinate
                    auto next_new_pos = vertex_positioning_engine.lock()->set_vertex_coordinate(vertex_descriptor,
                                                                                                new_pos.x(),
                                                                                                new_pos.y());
                    new_pos = QPointF(qreal(next_new_pos.first),
                                      qreal(next_new_pos.second));

                }
                return new_pos;
            }

            return QGraphicsItem::itemChange(change, value);
        }

    };
}

#endif