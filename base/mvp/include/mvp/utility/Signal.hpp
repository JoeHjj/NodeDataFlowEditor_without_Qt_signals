#pragma once
#include <functional>
#include <vector>

namespace base::mvp::utility
{
    /**
     * @brief A simple signal mechanism for connecting multiple slots.
     *
     * @tparam Args Parameter types for the signal.
     */
    template <typename... Args>
    class Signal
    {
    public:
        using SlotType = std::function<void(Args...)>;

        /**
         * @brief Connect a slot to the signal.
         * @param slot The callable to connect.
         * @return An ID that can be used to disconnect this slot.
         */
        size_t connect(SlotType slot)
        {
            m_slots.push_back(std::move(slot));
            return m_slots.size() - 1;
        }

        /**
         * @brief Disconnect a slot by its ID.
         * @param id ID of the slot to disconnect.
         */
        void disconnect(size_t id)
        {
            if (id < m_slots.size())
            {
                m_slots[id] = nullptr;
            }
        }

        /**
         * @brief Disconnect all connected slots.
         */
        void disconnectAll()
        {
            for (auto& slot : m_slots)
            {
                slot = nullptr;
            }
        }

        /**
         * @brief Block or unblock signal emissions.
         * @param block If true, signals will not notify until unblocked.
         */
        void blockSignals(bool block) { m_blocked = block; }

        /**
         * @brief notify the signal to all connected slots.
         * @param args Arguments to pass to each slot.
         */
        void notify(Args... args)
        {
            if (m_blocked)
                return;
            for (auto& slot : m_slots)
            {
                if (slot)
                    slot(args...);
            }
        }

    private:
        std::vector<SlotType> m_slots;
        bool m_blocked = false;
    };

    template <typename T>
    class Property
    {
    public:
        Signal<const T&> changed;

        Property() = default;
        explicit Property(T value)
            : m_value(std::move(value))
        {}

        const T& get() const { return m_value; }

        void set(const T& value)
        {
            if (value != m_value)
            {
                m_value = value;
                changed.notify(m_value);
            }
        }

    private:
        T m_value{};
    };
} // namespace base::mvp::utility
