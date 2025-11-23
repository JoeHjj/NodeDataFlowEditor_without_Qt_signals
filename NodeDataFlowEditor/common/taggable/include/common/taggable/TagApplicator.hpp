/*
    MIT License

    Copyright (c) 2025 Joseph Al Hajjar

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/

#pragma once

#include "common/taggable/TagRegistry.hpp"
#include "common/taggable/Taggable.hpp"
#include <functional>
#include <string>
#include <unordered_map>

namespace nodeeditor::common::taggable
{

    /**
     * @class TagApplicator
     * @brief Provides runtime tag registration and application mechanisms.
     *
     * The TagApplicator class allows associating string identifiers with compile-time tag types,
     * enabling runtime tag creation and application to Taggable objects.
     *
     * Typical usage involves registering all tag types at startup and later applying them by name.
     *
     * Example:
     * @code
     * TagApplicator::registerTag<MyTag>();
     * TagApplicator::apply("MyTag", myTaggableObject);
     * @endcode
     */
    class TagApplicator
    {
    public:
        /**
         * @brief Registers a tag type in the TagRegistry for runtime use.
         *
         * Associates the tag's string name with a callable that can apply it
         * to a Taggable object at runtime.
         *
         * @tparam Tag The tag type to register.
         */
        template <typename Tag>
        static void registerTag()
        {
            auto name = TagRegistry::getTagName<Tag>();
            size_t idx = TagRegistry::getTagIndex<Tag>();
            nameToIndex[name] = idx;

            indexToApply[idx] = [](Taggable& t) {
                t.addTag<Tag>();
            };
        }

        /**
         * @brief Applies a registered tag to a given Taggable object.
         *
         * Looks up the tag by name and, if found, applies it to the provided object.
         *
         * @param tagName The string name of the tag to apply.
         * @param t Reference to the Taggable object to modify.
         * @return True if the tag was successfully applied, false if the tag name was not found.
         */
        static bool apply(const std::string& tagName, Taggable& t)
        {
            auto it = nameToIndex.find(tagName);
            if (it == nameToIndex.end())
                return false;

            size_t idx = it->second;
            indexToApply[idx](t);
            return true;
        }

        /**
         * @brief Helper structure for registering multiple tag types at once.
         *
         * Use this template to register a list of tag types in one declaration.
         *
         * Example:
         * @code
         * static TagApplicator::MultiTagRegistrar<TagA, TagB, TagC> registrar;
         * @endcode
         *
         * @tparam Tags Variadic list of tag types to register.
         */
        template <typename... Tags>
        struct MultiTagRegistrar
        {
            /** @brief Constructor that registers all tag types in the parameter pack. */
            MultiTagRegistrar()
            {
                if constexpr (sizeof...(Tags) > 0)
                {
                    (TagApplicator::registerTag<Tags>(), ...);
                }
            }
        };

    private:
        /** @brief Maps tag names to their unique indices. */
        static inline std::unordered_map<std::string, size_t> nameToIndex;

        /** @brief Maps tag indices to their corresponding application functions. */
        static inline std::unordered_map<size_t, std::function<void(Taggable&)>> indexToApply;
    };

} // namespace nodeeditor::common::taggable
