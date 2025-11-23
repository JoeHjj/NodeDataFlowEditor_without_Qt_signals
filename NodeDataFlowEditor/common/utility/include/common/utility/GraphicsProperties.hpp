#pragma once

#include <cstdlib>

namespace nodeeditor::common::utility
{

    /**
     * @struct SPos
     * @brief Represents a 2D position in Cartesian coordinates.
     *
     * This structure stores the X and Y coordinates of a point in 2D space.
     * It supports copy/move semantics and comparison operators.
     */
    struct SPos
    {
        /** @brief X coordinate. */
        double x = 0.;

        /** @brief Y coordinate. */
        double y = 0.;

        /** @brief Default constructor. */
        SPos() = default;
        explicit SPos(double newX, double newY)
            : x(newX)
            , y(newY)
        {}

        /** @brief Copy constructor. */
        SPos(const SPos& other)
            : x(other.x)
            , y(other.y)
        {}

        /** @brief Move constructor. */
        SPos(SPos&& other) noexcept
            : x(other.x)
            , y(other.y)
        {
            other.x = 0.;
            other.y = 0.;
        }

        /** @brief Copy assignment operator. */
        SPos& operator=(const SPos& other)
        {
            if (this != &other)
            {
                x = other.x;
                y = other.y;
            }
            return *this;
        }

        /** @brief Move assignment operator. */
        SPos& operator=(SPos&& other) noexcept
        {
            if (this != &other)
            {
                x = other.x;
                y = other.y;
                other.x = 0.;
                other.y = 0.;
            }
            return *this;
        }

        /**
         * @brief Equality comparison operator.
         * @param other The SPos instance to compare with.
         * @return True if both positions are equal.
         */
        bool operator==(const SPos& other) const
        {
            constexpr double epsilon = 1e-6; // small tolerance
            return std::abs(x - other.x) < epsilon &&
                   std::abs(y - other.y) < epsilon;
        }

        /**
         * @brief Inequality comparison operator.
         * @param other The SPos instance to compare with.
         * @return True if the positions are not equal.
         */
        bool operator!=(const SPos& other) const
        {
            return !(*this == other);
        }
    };
    /**
     * @struct SPoint
     * @brief Represents a 2D point
     *
     * This structure stores the X and Y coordinates of a point in 2D space.
     * It supports copy/move semantics and comparison operators.
     */
    struct SPoint
    {
        /** @brief X coordinate. */
        double x = 0.;

        /** @brief Y coordinate. */
        double y = 0.;

        /** @brief Default constructor. */
        SPoint() = default;

        /** @brief Copy constructor. */
        SPoint(const SPoint& other)
            : x(other.x)
            , y(other.y)
        {}

        /** @brief Move constructor. */
        SPoint(SPoint&& other) noexcept
            : x(other.x)
            , y(other.y)
        {
            other.x = 0.;
            other.y = 0.;
        }

        /** @brief Copy assignment operator. */
        SPoint& operator=(const SPoint& other)
        {
            if (this != &other)
            {
                x = other.x;
                y = other.y;
            }
            return *this;
        }

        /** @brief Move assignment operator. */
        SPoint& operator=(SPoint&& other) noexcept
        {
            if (this != &other)
            {
                x = other.x;
                y = other.y;
                other.x = 0.;
                other.y = 0.;
            }
            return *this;
        }

        /**
         * @brief Equality comparison operator.
         * @param other The SPoint instance to compare with.
         * @return True if both positions are equal.
         */
        bool operator==(const SPoint& other) const
        {
            constexpr double epsilon = 1e-6; // small tolerance
            return std::abs(x - other.x) < epsilon &&
                   std::abs(y - other.y) < epsilon;
        }

        /**
         * @brief Inequality comparison operator.
         * @param other The SPoint instance to compare with.
         * @return True if the positions are not equal.
         */
        bool operator!=(const SPoint& other) const
        {
            return !(*this == other);
        }
    };

    /**
     * @struct SRect
     * @brief Represents a rectangle with position and dimensions.
     *
     * This structure defines a rectangular region in 2D space using X and Y
     * coordinates for the top-left corner and width/height for size.
     * It provides copy/move semantics and equality operators.
     */
    struct SRect
    {
        /** @brief X coordinate of the top-left corner. */
        double x = 0.;

        /** @brief Y coordinate of the top-left corner. */
        double y = 0.;

        /** @brief Rectangle width. */
        double width = 0.;

        /** @brief Rectangle height. */
        double height = 0.;

        /** @brief Default constructor. */
        SRect() = default;

        /** @brief Copy constructor. */
        SRect(double nx, double ny, double nwidth, double nheight)
            : x(nx)
            , y(ny)
            , width(nwidth)
            , height(nheight)
        {}
        /** @brief Copy constructor. */
        SRect(const SRect& other)
            : x(other.x)
            , y(other.y)
            , width(other.width)
            , height(other.height)
        {}

        /** @brief Move constructor. */
        SRect(SRect&& other) noexcept
            : x(other.x)
            , y(other.y)
            , width(other.width)
            , height(other.height)
        {
            other.x = 0.;
            other.y = 0.;
            other.width = 0.;
            other.height = 0.;
        }

        /** @brief Copy assignment operator. */
        SRect& operator=(const SRect& other)
        {
            if (this != &other)
            {
                x = other.x;
                y = other.y;
                width = other.width;
                height = other.height;
            }
            return *this;
        }

        /** @brief Move assignment operator. */
        SRect& operator=(SRect&& other) noexcept
        {
            if (this != &other)
            {
                x = other.x;
                y = other.y;
                width = other.width;
                height = other.height;
                other.x = 0.;
                other.y = 0.;
                other.width = 0.;
                other.height = 0.;
            }
            return *this;
        }

        /**
         * @brief Equality comparison operator.
         * @param other The SRect instance to compare with.
         * @return True if all position and size members are equal.
         */
        bool operator==(const SRect& other) const
        {
            constexpr double epsilon = 1e-6; // adjust as needed
            return std::abs(x - other.x) < epsilon &&
                   std::abs(y - other.y) < epsilon &&
                   std::abs(width - other.width) < epsilon &&
                   std::abs(height - other.height) < epsilon;
        }

        /**
         * @brief Inequality comparison operator.
         * @param other The SRect instance to compare with.
         * @return True if any of the position or size members differ.
         */
        bool operator!=(const SRect& other) const
        {
            return !(*this == other);
        }
    };

    /**
     * @struct SRotate
     * @brief Represents a rotation value in degrees.
     *
     * Used to define angular transformations around a given origin.
     */
    struct SRotate
    {
        /** @brief Rotation angle in degrees. */
        double angle = 0.;
    };

    /**
     * @struct SScale
     * @brief Represents a scale factor.
     *
     * Used to define uniform or proportional scaling transformations.
     */
    struct SScale
    {
        /** @brief Scale multiplier, where 1.0 represents 100% (no scaling). */
        double factor = 1.;
    };

} // namespace nodeeditor::common::utility
