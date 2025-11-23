#pragma once
#include "common/presenter/AbstractItemPresenter.hpp"

#include <map>
#include <memory>
#include <string>
#include <vector>

namespace nodeeditor::core::model
{
    class NodeItemModel;
}

namespace nodeeditor::core::view
{
    class NodeItemView;
}

namespace nodeeditor::core::presenter
{
    class PortItemPresenter;

    class NodeItemPresenter : public common::presenter::AbstractItemPresenter
    {
    public:
        NodeItemPresenter(std::shared_ptr<model::NodeItemModel> model,
                          std::shared_ptr<view::NodeItemView> view);
        ~NodeItemPresenter() override;

        // ---------------- Port helpers ----------------
        void addInputPort(const std::string& name, const std::string& displayName = "");
        void addOutputPort(const std::string& name, const std::string& displayName = "");
        void addParameterPort(const std::string& type, const std::string& name, const std::string& displayName = "");
        void removeInputPort(const std::string& name);
        void removeOutputPort(const std::string& name);
        void removeParameterPort(const std::string& name);
        void addPortPresenter(const std::shared_ptr<PortItemPresenter>& presenter);

        void removePortPresenter(const std::shared_ptr<PortItemPresenter>& presenter);

        std::shared_ptr<PortItemPresenter> getInputPort(const std::string& name) const;

        std::shared_ptr<PortItemPresenter> getOutputPort(const std::string& name) const;

        std::shared_ptr<PortItemPresenter> getParameterPort(const std::string& name) const;

        std::vector<std::shared_ptr<PortItemPresenter>> ports() const;

    private:
        std::map<std::string, std::shared_ptr<PortItemPresenter>> inputPortPresenters;
        std::map<std::string, std::shared_ptr<PortItemPresenter>> outputPortPresenters;
        std::map<std::string, std::shared_ptr<PortItemPresenter>> parameterPortPresenters;
    };

} // namespace nodeeditor::core::presenter
