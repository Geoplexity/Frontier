#ifndef FFNX_UI_GRAPH_EDGE
#define FFNX_UI_GRAPH_EDGE

#include "ffnx_ui/graph/logic/controller.h"
#include <QGraphicsItem>
#include <iostream>

namespace ffnx::ui::graph {

    template <typename TGraph>
    class Edge : public QGraphicsItem {
    private:
        std::weak_ptr<Controller<TGraph>> controller;

        using edge_desc = typename TGraph::edge_descriptor;

        edge_desc edge;

        QPointF source;
        QPointF dest;

    public:
        enum {
            Type = UserType + 1
        };
        int type() const override {
            return Type;
        }

        Edge(const edge_desc& edge,
               QWidget* parent = nullptr) : controller(), edge(edge), QGraphicsItem() {
            setFlag(ItemIsSelectable);
            setFlag(ItemSendsGeometryChanges);

            setZValue(-1);
        }

        void set_controller(std::shared_ptr<Controller<TGraph>> controller) {
            this->controller = controller;
        }

        void recompute_endpoints() {
            prepareGeometryChange();

            auto source_coords = this->controller.lock()->positioning_engine().get_edge_begin(edge);
            auto dest_coords = this->controller.lock()->positioning_engine().get_edge_end(edge);

            source = QPointF(source_coords.first, source_coords.second);
            dest = QPointF(dest_coords.first, dest_coords.second);
        }

        edge_desc edge_descriptor() const {
            return edge;
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
            prepareGeometryChange();

            auto controller_lk = controller.lock();
            if (controller_lk == nullptr) {
                return QGraphicsItem::itemChange(change, value);
            }

            if (controller_lk->is_event_loop_running()) {
                return QGraphicsItem::itemChange(change, value);
            } else {
                if (change == ItemSelectedChange) {
                    auto selection_state = value.value<bool>();
                    controller_lk->push_command(
                            std::move(std::make_unique<EdgeSelectionUICommand<TGraph>>(edge, selection_state)));
                }
            }

            return QGraphicsItem::itemChange(change, value);
        }
    };
}

#endif