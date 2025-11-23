#pragma once

#include "common/utility/GraphicsProperties.hpp"
#include "mvp/utility/Signal.hpp"
#include "mvp/view/IViewItem.hpp"
#include <QGraphicsItem>

namespace nodeeditor::common::view
{
    /**
     * @class AbstractItemView
     * @brief QGraphicsItem-based view for node editor items.
     *
     * Provides change-notification signals and optional std::function callbacks
     * for model-synchronized properties (rotation, scale, position, rect, etc.).
     *
     * Setters store the value, emit the corresponding Signal<...>, and may invoke
     * the matching onXChanged callback if it is set.
     */
    class AbstractItemView : public base::mvp::view::IViewItem, public QGraphicsItem
    {
        Q_INTERFACES(QGraphicsItem)

    public:
        /**
         * @brief Construct the view.
         * @param parent Optional QGraphicsItem parent.
         */
        explicit AbstractItemView(QGraphicsItem* parent = nullptr);
        ~AbstractItemView() override = default;
        /**
         * @brief Set the drawing color used for text and primitives.
         * @param c QColor to use.
         *
         * Note: This class does not expose a Signal for color, but you can use
         * @ref onColorChanged as a callback hook if needed.
         */
        void setColor(const QColor& c);

        // -------------------- Signals & Setters --------------------
        /** @brief Emitted when rotation changes. */
        base::mvp::utility::Signal<const double&> rotation_changed;
        /** @brief Set rotation (degrees) and emit @ref rotation_changed. */
        void set_rotation(double r);

        /** @brief Emitted when scale changes. */
        base::mvp::utility::Signal<const double&> scale_changed;
        /** @brief Set scale factor and emit @ref scale_changed. */
        void set_scale(double s);

        /** @brief Emitted when position changes. */
        base::mvp::utility::Signal<const utility::SPos&> pos_changed;
        /** @brief Set position and emit @ref pos_changed. */
        void set_pos(const utility::SPos& p);
        void set_pos(const double& x, const double& y);

        /** @brief Emitted when rectangle changes. */
        base::mvp::utility::Signal<const utility::SRect&> rect_changed;
        /** @brief Set rectangle and emit @ref rect_changed. */
        void set_rect(const utility::SRect& r);

        /** @brief Emitted when enabled state changes. */
        base::mvp::utility::Signal<const bool&> enable_changed;
        /** @brief Set enabled state and emit @ref enable_changed. */
        void set_enable(bool e);

        /** @brief Emitted when visibility changes. */
        base::mvp::utility::Signal<const bool&> visible_changed;
        /** @brief Set visibility and emit @ref visible_changed. */
        void set_visible(bool v);

        /** @brief Emitted when active state changes. */
        base::mvp::utility::Signal<const bool&> active_changed;
        /** @brief Set active state and emit @ref active_changed. */
        void set_active(bool a);

        /** @brief Emitted when selection changes. */
        base::mvp::utility::Signal<const bool&> select_changed;
        /** @brief Set selection state and emit @ref select_changed. */
        void set_select(bool s);

        /** @brief Emitted when hovered changes. */
        base::mvp::utility::Signal<const bool&> hovered_changed;
        /** @brief Set hovered state and emit @ref hovered_changed. */
        void set_hovered(bool s);

        /** @brief Emitted when mouse pressed changes. */
        base::mvp::utility::Signal<const bool&> pressed_changed;
        /** @brief Set mouse pressed state and emit @ref pressed_changed. */
        void set_pressed(bool s);

        /** @brief Emitted when mouse double clicked changes. */
        base::mvp::utility::Signal<const bool&> double_clicked_changed;
        /** @brief Set mouse doubleClicked state and emit @ref set_double_clicked. */
        void set_double_clicked(bool s);

        /** @brief Emitted when item is moving. */
        base::mvp::utility::Signal<const bool&> moving_changed;
        /** @brief Set moving state and emit @ref moving_changed. */
        void set_moving(bool s);

        // -------------------- Optional Callbacks --------------------
        void setOnColorChanged(std::function<void(const QColor&)> cb);
        void setOnRotationChanged(std::function<void(const double&)> cb);
        void setOnScaleChanged(std::function<void(const double&)> cb);
        void setOnPosChanged(std::function<void(const utility::SPos&)> cb);
        void setOnRectChanged(std::function<void(const utility::SRect&)> cb);
        void setOnEnableChanged(std::function<void(const bool&)> cb);
        void setOnVisibleChanged(std::function<void(const bool&)> cb);
        void setOnActiveChanged(std::function<void(const bool&)> cb);
        void setOnSelectChanged(std::function<void(const bool&)> cb);
        void setOnHoverChanged(std::function<void(const bool&)> cb);
        void setOnPressedChanged(std::function<void(const bool&)> cb);
        void setOnDoubleClickedChanged(std::function<void(const bool&)> cb);
        void setOnMovingChanged(std::function<void(const bool&)> cb);

        static QRectF toQRectF(const utility::SRect& r);
        static utility::SRect fromQRectF(const QRectF& r);

        /** @brief Item bounds in local coordinates. */
        QRectF boundingRect() const override;

    protected:
        /** @brief Paint the item. */
        void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) override;
        bool sceneEvent(QEvent* event) override;
        QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;

    protected:
        QColor color_{Qt::white};
        QRectF m_rect;
        double rotation_{0.0};
        double scale_{1.0};
        utility::SPos pos_;
        utility::SRect rect_;
        bool enable_{true};
        bool visible_{true};
        bool active_{false};
        bool select_{false};
        bool hovered_{false};
        bool pressed_{false};
        bool double_clicked_{false};
        bool moving_{false};

    private:
        /** @brief Called after color changes in @ref setColor. */
        std::function<void(const QColor&)> onColorChanged;

        /** @brief Called after rotation changes in @ref set_rotation. */
        std::function<void(const double&)> onRotationChanged;

        /** @brief Called after scale changes in @ref set_scale. */
        std::function<void(const double&)> onScaleChanged;

        /** @brief Called after position changes in @ref set_pos. */
        std::function<void(const utility::SPos&)> onPosChanged;

        /** @brief Called after rectangle changes in @ref set_rect. */
        std::function<void(const utility::SRect&)> onRectChanged;

        /** @brief Called after enabled state changes in @ref set_enable. */
        std::function<void(const bool&)> onEnableChanged;

        /** @brief Called after visibility changes in @ref set_visible. */
        std::function<void(const bool&)> onVisibleChanged;

        /** @brief Called after active state changes in @ref set_active. */
        std::function<void(const bool&)> onActiveChanged;

        /** @brief Called after selection changes in @ref set_select. */
        std::function<void(const bool&)> onSelectChanged;

        /** @brief Called after hovered changes in @ref set_hovered. */
        std::function<void(const bool&)> onHoverChanged;

        /** @brief Called after pressed changes in @ref set_pressed. */
        std::function<void(const bool&)> onPressedChanged;

        /** @brief Called after doubleClicked changes in @ref set_double_clicked. */
        std::function<void(const bool&)> onDoubleClickedChanged;

        /** @brief Called after moving status changes in @ref set_moving. */
        std::function<void(const bool&)> onMovingChanged;
    };

} // namespace nodeeditor::common::view
