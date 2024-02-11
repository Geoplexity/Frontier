#ifndef FFNX_UI_GRAPH_VERTEX
#define FFNX_UI_GRAPH_VERTEX

#include "ffnx_ui/graph/logic/controller.h"
#include <QGraphicsItem>
#include <iostream>

namespace ffnx::ui::graph {

    template <typename TGraph>
    class Vertex : public QGraphicsItem {
    private:
        std::weak_ptr<Controller<TGraph>> controller;

        using vert_desc = typename TGraph::vertex_descriptor;

        float radius = 0;

    public:
        enum {
            Type = UserType + 1
        };
        int type() const override {
            return Type;
        }

        vert_desc vert;

        Vertex(const vert_desc& vert,
               QWidget* parent = nullptr) : controller(), vert(vert), QGraphicsItem() {
            setFlag(ItemIsMovable);
            setFlag(ItemIsSelectable);
            setFlag(ItemSendsGeometryChanges);
            setCacheMode(DeviceCoordinateCache);

        }

        void set_controller(std::shared_ptr<Controller<TGraph>> controller) {
            this->controller = controller;
            prepareGeometryChange();
            this->radius = controller->positioning_engine().get_vertex_radius();
        }

        vert_desc vertex_descriptor() const {
            return vert;
        }

        QRectF boundingRect() const override {
            return QRectF(-radius, -radius, 2 * radius, 2 * radius);
        }

        QPainterPath shape() const override {
            QPainterPath path;
            path.addEllipse(-radius, -radius, 2 * radius, 2 * radius);
            return path;
        }

        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override {
            auto pen = isSelected() ? QPen(Qt::red, 3) : QPen(Qt::black);

            painter->setPen(pen);
            painter->setBrush(Qt::white);
            painter->drawEllipse(-radius, -radius, 2 * radius, 2 * radius);
        }

        void recompute_position() {
            auto new_position = controller.lock()->positioning_engine().get_vertex_coordinate(vert);
            setPos(new_position.first, new_position.second);
        }

        QVariant itemChange(GraphicsItemChange change, const QVariant &value) override {


            auto controller_lk = controller.lock();

            if (!(controller_lk == nullptr)) {

                // if the event loop is running, do not schedule additional events
                if (controller_lk->is_event_loop_running()) {
                    return QGraphicsItem::itemChange(change, value);
                }

                if (change == ItemPositionChange) {
                    auto updated_position = value.value<QPointF>();
                    auto x = updated_position.x();
                    auto y = updated_position.y();
                    controller_lk->push_command(std::move(std::make_unique<VertexPositioningUICommand<TGraph>>(
                            vert, x, y)));
                } else if (change == ItemSelectedChange) {
                    auto selection_state = value.value<bool>();
                    controller_lk->push_command(std::move(std::make_unique<VertexSelectionUICommand<TGraph>>(
                            vert, selection_state)));
                }
            }

            return QGraphicsItem::itemChange(change, value);
        }

    };
}

#endif