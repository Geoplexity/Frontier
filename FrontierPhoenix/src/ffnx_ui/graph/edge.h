#ifndef FFNX_UI_GRAPH_EDGE
#define FFNX_UI_GRAPH_EDGE

#include "ffnx_ui/graph/logic/controller.h"
#include <QGraphicsItem>
#include <iostream>
#include <algorithm>

#include <boost/graph/graph_selectors.hpp>

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

            auto vertex_radius = this->controller.lock()->positioning_engine().get_vertex_radius();

            source = QPointF(source_coords.first, source_coords.second);
            dest = QPointF(dest_coords.first, dest_coords.second);

            QLineF line(source, dest);
            line.setLength(line.length() - vertex_radius);
            line = QLineF(line.p2(), line.p1());
            line.setLength(line.length() - vertex_radius);

            source = line.p1();
            dest = line.p2();

        }

        edge_desc edge_descriptor() const {
            return edge;
        }

        [[nodiscard]] QRectF boundingRect() const override {
            auto x_min = std::min(source.x(), dest.x());
            auto y_min = std::min(source.y(), dest.y());

            auto width = std::abs(source.x() - dest.x());
            auto height = std::abs(source.y() - dest.y());

            return QRectF(x_min - 5, y_min - 5, width + 10, height + 10);
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

            painter->setPen(pen);
            painter->setBrush(Qt::black);


            QPainterPath path(source);
            path.lineTo(dest);
            painter->drawPath(path);

            if(!(typename TGraph::directionality().is_directed)) {
                return;
            }

            QLineF delta(source, dest);

            auto arrow_head_length = std::min(qreal(5), delta.length());

            auto arrow_start = QLineF(dest, source);
            arrow_start.setLength(arrow_head_length);
            auto arrow_start_1 = QLineF(dest, arrow_start.p2());
            arrow_start_1.setAngle(arrow_start_1.angle() - 20);
            auto arrow_start_2 = QLineF(dest, arrow_start.p2());
            arrow_start_2.setAngle(arrow_start_2.angle() + 20);

            path = QPainterPath(arrow_start_1.p2());
            path.lineTo(dest);
            painter->drawPath(path);

            path = QPainterPath(arrow_start_2.p2());
            path.lineTo(dest);
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