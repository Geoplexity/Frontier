#ifndef FFNX_UI_GRAPH_EDGE
#define FFNX_UI_GRAPH_EDGE

#include <QGraphicsItem>
#include <iostream>
#include "graph_selection_model.h"

namespace ffnx::ui::graph {

    template <typename TGraph>
    class Edge : public QGraphicsItem {
    private:
        using edge_desc = typename TGraph::edge_descriptor;

        std::weak_ptr<VertexPositioningEngine<TGraph>> vertex_positioning_engine;

        std::weak_ptr<GraphSelectionModel<TGraph>> selection_model;

        edge_desc edge;

        QPointF source;
        QPointF dest;

    public:
        enum {
            Type = UserType + 2
        };
        int type() const override {
            return Type;
        }

        Edge(std::weak_ptr<VertexPositioningEngine<TGraph>> vertex_positioning_engine,
             std::weak_ptr<GraphSelectionModel<TGraph>> selection_model,
             const edge_desc &edge) :
                vertex_positioning_engine(vertex_positioning_engine),
                selection_model(selection_model),
                edge(edge) {
            setFlag(ItemIsSelectable);


            auto points = get_points();
            source = points.first;
            dest = points.second;

            // callback to notify the edge when it is modified
            vertex_positioning_engine.lock()->add_edge_listener(edge, [this](){
                this->reposition();
            });

            setZValue(-1);
        }

        void reposition() {
            prepareGeometryChange();

            auto points = get_points();
            source = points.first;
            dest = points.second;
        }

        [[nodiscard]] QRectF boundingRect() const override {
            return QRectF(source, dest);
        }

        [[nodiscard]] QPainterPath shape() const override {
            QPainterPath path(source);
            path.lineTo(dest);

            QPainterPathStroker stroker;
            stroker.setWidth(6);

            return stroker.createStroke(path);
        }

        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override {
            auto pen = isSelected() ? QPen(Qt::red, 2, Qt::DashLine) : QPen(Qt::black, 2);

            QPainterPath path(source);
            path.lineTo(dest);

            painter->setPen(pen);
            painter->setBrush(Qt::black);
            painter->drawPath(path);
        }

        QVariant itemChange(GraphicsItemChange change, const QVariant &value) override {

            if (change == ItemSelectedChange) {
                prepareGeometryChange();

                auto selection_state = value.value<bool>();
                if (selection_state) {
                    selection_model.lock()->select_edge(edge);
                } else {
                    selection_model.lock()->deselect_edge(edge);
                }
            }

            return QGraphicsItem::itemChange(change, value);
        }

    private:
        [[nodiscard]] std::pair<QPointF, QPointF> get_points() const {
            auto v0 = vertex_positioning_engine.lock()->get_edge_begin(edge);
            auto v1 = vertex_positioning_engine.lock()->get_edge_end(edge);

            auto x0 = v0.first;
            auto y0 = v0.second;

            auto x1 = v1.first;
            auto y1 = v1.second;

            return { QPointF(qreal(x0), qreal(y0)), QPointF(qreal(x1), qreal(y1)) };
        }
    };
}

#endif