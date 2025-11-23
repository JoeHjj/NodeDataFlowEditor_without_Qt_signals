#pragma once

#include "common/view/AbstractItemView.hpp"
#include <QGraphicsProxyWidget>
#include <QLineEdit>
#include <QPainter>

namespace nodeeditor::core::view
{
    class EditableArrowItemView;
    class PortItemView;

    class NodeItemView : public nodeeditor::common::view::AbstractItemView
    {
    public:
        explicit NodeItemView(QString nodeName,
                              const QString& nodeDisplayedName,
                              QColor titleColor,
                              QGraphicsItem* parent = nullptr);

        explicit NodeItemView(QString nodeName,
                              const QString& nodeDisplayedName = "",
                              QGraphicsItem* parent = nullptr);

        ~NodeItemView() override;

        QVector<std::shared_ptr<PortItemView>> getAllPorts() const;

        std::shared_ptr<PortItemView> addInput(const QString& name);
        std::shared_ptr<PortItemView> addOutput(const QString& name);
        std::shared_ptr<PortItemView> addInput(const QString& name, const QString& displayName);
        std::shared_ptr<PortItemView> addOutput(const QString& name, const QString& displayName);

        void removeInput(const QString& name);
        void removeOutput(const QString& name);
        void removeParamInput(const QString& name);

        void removeInput(const std::shared_ptr<PortItemView>& input);
        void removeOutput(const std::shared_ptr<PortItemView>& output);
        void removeParamInput(const std::shared_ptr<PortItemView>& input);

        QRectF boundingRect() const override;
        void paint(QPainter* painter,
                   const QStyleOptionGraphicsItem*,
                   QWidget*) override;

        QVector<std::shared_ptr<PortItemView>> inputs() const;
        QVector<std::shared_ptr<PortItemView>> outputs() const;

        void changeVisibility(bool val);
        void changeNodeVisibility(bool val);

        std::shared_ptr<PortItemView> addParameter(QWidget* widget, const QString& name);
        std::shared_ptr<PortItemView> addParameter(QWidget* widget, const QString& name, const QString& displayName);

        QMap<QWidget*, QGraphicsProxyWidget*> parameterWidgets() const;
        QWidget* getParameterWidget(std::shared_ptr<PortItemView>) const;
        QMap<std::shared_ptr<PortItemView>, QGraphicsProxyWidget*> parameterPorts() const;
        QList<std::shared_ptr<PortItemView>> paramsInputs() const;

        void setDisplayedNodeName(const QString& t);
        QString displayedNodeName() const;
        void setNodeName(const QString& t);
        QString nodeName() const;
        void setNodeNameColor(const QColor& c);

        void disconnectAllPorts();
        std::shared_ptr<PortItemView> getPort(const QGraphicsProxyWidget& poxy) const;
        void updateLayout();

        base::mvp::utility::Signal<const std::string&> text_changed;
        void set_text(const std::string& t);
        std::shared_ptr<PortItemView> addPortView(
            const std::shared_ptr<PortItemView>& port,
            QWidget* widget = nullptr);

    protected:
        void updateRect();

    private:
        std::shared_ptr<PortItemView> addParamInput(const QString& name);

        void drawBackground(QPainter& painter) const;
        void drawTitle(QPainter& painter) const;
        void drawGlowingBounding(QPainter& painter);
        void setActive(bool newIsActive);

    private:
        EditableArrowItemView* m_nodeNameLabel = nullptr;
        QString m_nodeName;
        QString m_displayedNodeName;
        QColor m_nodeNameColor;

        QVector<std::shared_ptr<PortItemView>> m_inputs;
        QVector<std::shared_ptr<PortItemView>> m_outputs;

        qreal m_titleHeight = 30;
        int m_margin = 20;
        int m_spacing = 20;
        int minWidth = 100;
        qreal m_maxInputWidth = 0;
        qreal m_maxOutputWidth = 0;
        qreal m_maxParamWidth = 0;

        QColor m_bgColor = QColor(30, 30, 30);
        QColor m_borderColor = QColor(70, 70, 70);

        QRectF m_rect;

        QMap<QWidget*, QGraphicsProxyWidget*> m_parameterWidgets;
        QMap<std::shared_ptr<PortItemView>, QGraphicsProxyWidget*> m_parameterPorts;

        QSize m_paramsRectSize;
    };

} // namespace nodeeditor::core::view
