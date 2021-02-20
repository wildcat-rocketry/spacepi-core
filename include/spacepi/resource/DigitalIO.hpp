#ifndef SPACEPI_CORE_RESOURCE_DIGITALIO_HPP
#define SPACEPI_CORE_RESOURCE_DIGITALIO_HPP

#include <functional>
#include <memory>
#include <ostream>
#include <string>

namespace spacepi {
    namespace resource {
        /**
         * \brief A hardware resource which represents a digital I/O port
         */
        class DigitalIO {
            public:
                /**
                 * \brief Enumeration representing the different modes in which a digital I/O port can be
                 */
                enum Mode {
                    /**
                     * \brief The pin is driven by the computer
                     */
                    Output = 0,
                    /**
                     * \brief The pin is not driven and is read by the computer
                     */
                    Input = 1,
                    /**
                     * \brief The output pin is in open drain or open collector mode
                     * 
                     * Open drain means the pin can be either left floating or shorted to ground, so an external pullup
                     * resistor may be required to get the proper logic level
                     */
                    OpenDrain = 2,
                    /**
                     * \brief The output pin is in open source or open emitter mode
                     * 
                     * Open source means the pin can be either left floating or shorted to power, so an extenral
                     * pulldown resistor may be required to get the proper logic level
                     */
                    OpenSource = 4,
                    /**
                     * \brief The input pin has an internal pullup resistor
                     * 
                     * This pullup resistor causes the input to be read as a high if the pin is left floating
                     */
                    PullUp = 2,
                    /**
                     * \brief The input pin has an internal pulldown resistor
                     * 
                     * This pulldown resistor causes the input to be read as a low if the pin is left floating
                     */
                    PullDown = 4
                };

                /**
                 * \brief Enumeration representing different interrupt edge modes
                 */
                enum Edge {
                    /**
                     * \brief The interrupt will never occur
                     */
                    Disable = 0,
                    /**
                     * \brief The interrupt will occur when the value changes from low to high
                     */
                    Rising = 1,
                    /**
                     * \brief The interrupt will occur when the value changes from high to low
                     */
                    Falling = 2,
                    /**
                     * \brief The interrupt will occur whenever the value changes
                     */
                    Transition = 3
                };

                /**
                 * \brief Destroy this DigitalIO resource
                 */
                virtual ~DigitalIO() = default;

                /**
                 * \brief Get a pointer to the DigitalIO object by its resource name
                 * 
                 * \param[in] name The resource name for which to get the DigitalIO object
                 * 
                 * \return The DigitalIO object, or a null pointer if the name is not registered
                 */
                static std::shared_ptr<DigitalIO> get(const std::string &name);

                /**
                 * \brief Read the value of the input pin
                 * 
                 * \return The current value (high or low)
                 */
                virtual operator bool() = 0;

                /**
                 * \brief Sets the value of an output pin
                 * 
                 * \param[in] value The new value (high or low)
                 * 
                 * \return \c this
                 */
                virtual DigitalIO &operator =(bool value) = 0;

                /**
                 * \brief Gets the current mode in which the port is operating
                 * 
                 * \return The mode
                 */
                virtual enum Mode getMode() const noexcept = 0;

                /**
                 * \brief Change the mode in which the port is operating
                 * 
                 * \param[in] mode The new mode
                 */
                virtual void setMode(enum Mode mode) = 0;

                /**
                 * \brief Gets the function which will be run when the interrupt triggers
                 * 
                 * \return The function
                 */
                virtual const std::function<void(void)> &getISR() const noexcept = 0;

                /**
                 * \brief Gets the Edge on which the interrupt will trigger
                 * 
                 * \return The edge
                 */
                virtual enum Edge getISREdge() const noexcept = 0;

                /**
                 * \brief Sets up an interrupt handler
                 * 
                 * \param[in] isr The function to call when the interrupt triggers
                 * \param[in] edge The Edge on which the interrupt will trigger
                 */
                virtual void setISR(const std::function<void(void)> &isr, enum Edge edge) = 0;

                /**
                 * \brief Removes the current interrupt handler
                 */
                void clearISR();
        };
    }
}

/**
 * \brief Helper method to prevent the need for a cast when bitwise or'ing two spacepi::resource::DigitalIO::Mode's
 * 
 * \param[in] a The LHS
 * \param[in] b The RHS
 * 
 * \return The bitwise or'ed value
 */
enum spacepi::resource::DigitalIO::Mode operator |(enum spacepi::resource::DigitalIO::Mode a, enum spacepi::resource::DigitalIO::Mode b) noexcept;

/**
 * \brief Helper method to prevent the need for a cast when bitwise or-assinging a spacepi::resource::DigitalIO::Mode
 * 
 * \param[in,out] a The LHS
 * \param[in] b The RHS
 * 
 * \return \c a
 */
enum spacepi::resource::DigitalIO::Mode &operator |=(enum spacepi::resource::DigitalIO::Mode &a, enum spacepi::resource::DigitalIO::Mode b) noexcept;

/**
 * \brief Writes the logical name of a spacepi::resource::DigitalIO::Mode enum constant to a stream
 * 
 * \param[in,out] os The stream to write to
 * \param[in] mode The enum constant
 * 
 * \return \c os
 */
std::ostream &operator <<(std::ostream &os, spacepi::resource::DigitalIO::Mode mode);

/**
 * \brief Writes the logical name of a spacepi::resource::DigitalIO::Edge enum constant to a stream
 * 
 * \param[in,out] os The stream to write to
 * \param[in] edge The enum constant
 * 
 * \return \c os
 */
std::ostream &operator <<(std::ostream &os, spacepi::resource::DigitalIO::Edge edge);

#endif
