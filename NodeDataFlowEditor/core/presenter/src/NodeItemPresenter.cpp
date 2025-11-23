#include "core/presenter/NodeItemPresenter.hpp"
#include "common/utility/ConnectionInfo.hpp"
#include "core/model/NodeItemModel.hpp"
#include "core/model/PortItemModel.hpp"
#include "core/presenter/PortItemPresenter.hpp"
#include "core/view/NodeItemView.hpp"
#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QSpinBox>

namespace nodeeditor::core::presenter
{

    NodeItemPresenter::NodeItemPresenter(std::shared_ptr<model::NodeItemModel> model,
                                         std::shared_ptr<view::NodeItemView> view)
        : common::presenter::AbstractItemPresenter(model, view)
    {
        auto* m = model.get();
        auto* v = view.get();

        // ---------------- Connect Model -> View ----------------
        m->text_changed.connect([v](const std::string& t) { v->set_text(t); });
        m->add_input.connect([v](const std::string& n, const std::string& d) { v->addInput(QString::fromStdString(n), QString::fromStdString(d)); });
        m->add_output.connect([v](const std::string& n, const std::string& d) { v->addOutput(QString::fromStdString(n), QString::fromStdString(d)); });
        m->remove_input.connect([v](const std::string& n) { v->removeInput(QString::fromStdString(n)); });
        m->remove_output.connect([v](const std::string& n) { v->removeOutput(QString::fromStdString(n)); });
        m->add_parameter.connect([v](const std::string& type, const std::string& n, const std::string& d) {
            if (type == "QString" || type == "string")
                v->addParameter(new QLineEdit(), QString::fromStdString(n), QString::fromStdString(d));
            else if (type == "bool")
                v->addParameter(new QCheckBox(), QString::fromStdString(n), QString::fromStdString(d));
            else if (type == "int")
                v->addParameter(new QSpinBox(), QString::fromStdString(n), QString::fromStdString(d));
            else if (type == "double" || type == "float")
                v->addParameter(new QDoubleSpinBox(), QString::fromStdString(n), QString::fromStdString(d));
        });
        m->remove_parameter.connect([v](const std::string& n) { v->removeParamInput(QString::fromStdString(n)); });

        // ---------------- Connect View -> Model ----------------
        v->text_changed.connect([m](const std::string& t) { m->set_text(t); });
    }

    NodeItemPresenter::~NodeItemPresenter() = default;

    // ---------------- Port helpers ----------------
    void NodeItemPresenter::addInputPort(const std::string& name, const std::string& displayName)
    {
        auto* m = dynamic_cast<model::NodeItemModel*>(m_model.get());
        if (m)
            m->add_input.notify(name, displayName.empty() ? name : displayName);
    }

    void NodeItemPresenter::addOutputPort(const std::string& name, const std::string& displayName)
    {
        auto* m = dynamic_cast<model::NodeItemModel*>(m_model.get());
        if (m)
            m->add_output.notify(name, displayName.empty() ? name : displayName);
    }

    void NodeItemPresenter::addParameterPort(const std::string& type, const std::string& name, const std::string& displayName)
    {
        auto* m = dynamic_cast<model::NodeItemModel*>(m_model.get());
        if (m)
            m->add_parameter.notify(type, name, displayName.empty() ? name : displayName);
    }

    void NodeItemPresenter::removeInputPort(const std::string& name)
    {
        auto* m = dynamic_cast<model::NodeItemModel*>(m_model.get());
        if (m)
            m->remove_input.notify(name);
    }

    void NodeItemPresenter::removeOutputPort(const std::string& name)
    {
        auto* m = dynamic_cast<model::NodeItemModel*>(m_model.get());
        if (m)
            m->remove_output.notify(name);
    }

    void NodeItemPresenter::removeParameterPort(const std::string& name)
    {
        auto* m = dynamic_cast<model::NodeItemModel*>(m_model.get());
        if (m)
            m->remove_parameter.notify(name);
    }

    void NodeItemPresenter::addPortPresenter(const std::shared_ptr<PortItemPresenter>& presenter)
    {
        auto portModel = std::static_pointer_cast<nodeeditor::core::model::PortItemModel>(presenter->model());

        if (!portModel)
            return;

        const std::string& portName = portModel->name();

        if (portModel->orientation() == common::utility::SPort::Orientation::Input)
            inputPortPresenters[portName] = presenter;
        else if (portModel->orientation() == common::utility::SPort::Orientation::Output)
            outputPortPresenters[portName] = presenter;
        else if (portModel->orientation() == common::utility::SPort::Orientation::Parameter)
            parameterPortPresenters[portName] = presenter;
    }

    void NodeItemPresenter::removePortPresenter(const std::shared_ptr<PortItemPresenter>& presenter)
    {
        auto portModel = std::static_pointer_cast<nodeeditor::core::model::PortItemModel>(presenter->model());
        if (!portModel)
            return;
        const std::string& portName = portModel->name();

        if (portModel->orientation() == common::utility::SPort::Orientation::Input)
            inputPortPresenters.erase(portName);
        else if (portModel->orientation() == common::utility::SPort::Orientation::Output)
            outputPortPresenters.erase(portName);
        else if (portModel->orientation() == common::utility::SPort::Orientation::Parameter)
            parameterPortPresenters.erase(portName);
    }

    std::shared_ptr<PortItemPresenter> NodeItemPresenter::getInputPort(const std::string& name) const
    {
        auto it = inputPortPresenters.find(name);
        return it != inputPortPresenters.end() ? it->second : nullptr;
    }

    std::shared_ptr<PortItemPresenter> NodeItemPresenter::getOutputPort(const std::string& name) const
    {
        auto it = outputPortPresenters.find(name);
        return it != outputPortPresenters.end() ? it->second : nullptr;
    }

    std::shared_ptr<PortItemPresenter> NodeItemPresenter::getParameterPort(const std::string& name) const
    {
        auto it = parameterPortPresenters.find(name);
        return it != parameterPortPresenters.end() ? it->second : nullptr;
    }

    std::vector<std::shared_ptr<PortItemPresenter>> NodeItemPresenter::ports() const
    {
        std::vector<std::shared_ptr<PortItemPresenter>> all;
        for (auto& [_, p] : inputPortPresenters)
            all.push_back(p);
        for (auto& [_, p] : outputPortPresenters)
            all.push_back(p);
        for (auto& [_, p] : parameterPortPresenters)
            all.push_back(p);
        return all;
    }

} // namespace nodeeditor::core::presenter
