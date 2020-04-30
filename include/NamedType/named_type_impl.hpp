#ifndef named_type_impl_h
#define named_type_impl_h

#include <type_traits>
#include <utility>

// Enable empty base class optimization with multiple inheritance on Visual Studio.
#if defined(_MSC_VER) && _MSC_VER >= 1910
#    define FLUENT_EBCO __declspec(empty_bases)
#else
#    define FLUENT_EBCO
#endif

#if defined(__clang__) || defined(__GNUC__)
#   define IGNORE_SHOULD_RETURN_REFERENCE_TO_THIS_BEGIN                                                                \
    _Pragma("GCC diagnostic push") _Pragma("GCC diagnostic ignored \"-Weffc++\"")
#   define IGNORE_SHOULD_RETURN_REFERENCE_TO_THIS_END _Pragma("GCC diagnostic pop")
#else
#   define IGNORE_SHOULD_RETURN_REFERENCE_TO_THIS_BEGIN /* Nothing */
#   define IGNORE_SHOULD_RETURN_REFERENCE_TO_THIS_END   /* Nothing */
#endif

#if defined(FLUENT_NODISCARD)
#error FLUENT_NODISCARD already defined
#endif

#if (__cplusplus >= 201703L) && !defined(_MSVC_LANG)
#if defined(__has_cpp_attribute) && (__has_cpp_attribute(nodiscard) >= 201603L)
#define FLUENT_NODISCARD [[nodiscard]]
#endif
#endif

#if defined(_MSVC_LANG) && (_MSVC_LANG>=201703L)
#define FLUENT_NODISCARD [[nodiscard]]
#endif

#if !defined(FLUENT_NODISCARD)
#define FLUENT_NODISCARD
#endif

namespace fluent
{

template <typename T>
using IsNotReference = typename std::enable_if<!std::is_reference<T>::value, void>::type;

template <typename T, typename Parameter, template <typename> class... Skills>
class FLUENT_EBCO NamedType : public Skills<NamedType<T, Parameter, Skills...>>...
{
public:
    using UnderlyingType = T;

    // constructor
    template <typename T_ = T, typename = std::enable_if<std::is_default_constructible<T>::value, void>>
    constexpr NamedType() noexcept(std::is_nothrow_constructible<T>::value)
    {
    }

    explicit constexpr NamedType(T const& value) noexcept(std::is_nothrow_copy_constructible<T>::value) : value_(value)
    {
    }

    template <typename T_ = T, typename = IsNotReference<T_>>
    explicit constexpr NamedType(T&& value) noexcept(std::is_nothrow_move_constructible<T>::value)
        : value_(std::move(value))
    {
    }

    // conversions
    using ref = NamedType<T&, Parameter, Skills...>;
    operator ref()
    {
        return ref(value_);
    }

    struct argument
    {
        template <typename U>
        NamedType operator=(U&& value) const
        {
            IGNORE_SHOULD_RETURN_REFERENCE_TO_THIS_BEGIN

            return NamedType(std::forward<U>(value));

            IGNORE_SHOULD_RETURN_REFERENCE_TO_THIS_END
        }

        argument() = default;
        argument(argument const&) = delete;
        argument(argument&&) = delete;
        argument& operator=(argument const&) = delete;
        argument& operator=(argument&&) = delete;
    };

    // get
    FLUENT_NODISCARD constexpr T& get() noexcept
    {
        return value_;
    }

    FLUENT_NODISCARD constexpr T const& get() const noexcept
    {
        return value_;
    }

private:
    T value_{};
};

template <template <typename T> class StrongType, typename T>
constexpr StrongType<T> make_named(T const& value)
{
    return StrongType<T>(value);
}

} // namespace fluent

#endif /* named_type_impl_h */
