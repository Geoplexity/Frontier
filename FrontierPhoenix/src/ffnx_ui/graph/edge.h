#ifndef FFNX_UI_GRAPH_EDGE
#define FFNX_UI_GRAPH_EDGE

#include <QGraphicsItem>
#include <iostream>

namespace ffnx::ui::graph {

    template <typename TGraph>
    class Edge : public QGraphicsItem {
    private:
        using edge_desc = typename TGraph::edge_descriptor;

        std::weak_ptr<VertexPositioningEngine<TGraph>> vertex_positioning_engine;

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
             const edge_desc &edge) : vertex_positioning_engine(vertex_positioning_engine), edge(edge) {
            setAcceptedMouseButtons(Qt::NoButton);

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

            return path;
        }

        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override {
            painter->setPen(QPen(Qt::black, 2));
            painter->setBrush(Qt::black);
            painter->drawPath(shape());
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