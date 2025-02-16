#pragma once
#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <type_traits>
#include <utility>
#include <cstddef>
#include <bitset>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonDocument>
#include <QString>

#define PP_THIRD_ARG(a, b, c, ...) c
#define VA_OPT_SUPPORTED_I(...) PP_THIRD_ARG(__VA_OPT__(, ), 1, 0, )
#define VA_OPT_SUPPORTED VA_OPT_SUPPORTED_I(?)

// Traditional MSVC requires a special EXPAND phase
#if (defined(_MSC_VER) && !defined(_MSVC_TRADITIONAL)) ||                  \
    (defined(_MSVC_TRADITIONAL) && _MSVC_TRADITIONAL)

#define GET_ARG_COUNT(...)                                                 \
    INTERNAL_EXPAND_ARGS_PRIVATE(INTERNAL_ARGS_AUGMENTER(__VA_ARGS__))

#define INTERNAL_ARGS_AUGMENTER(...) unused, __VA_ARGS__
#define INTERNAL_EXPAND(x) x
#define INTERNAL_EXPAND_ARGS_PRIVATE(...)                                  \
    INTERNAL_EXPAND(INTERNAL_GET_ARG_COUNT_PRIVATE(                        \
        __VA_ARGS__, 100, 99, 98, 97, 96, 95, 94, 93, 92, 91, 90, 89, 88,  \
        87, 86, 85, 84, 83, 82, 81, 80, 79, 78, 77, 76, 75, 74, 73, 72,    \
        71, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56,    \
        55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40,    \
        39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24,    \
        23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7,   \
        6, 5, 4, 3, 2, 1, 0))

#else  // Other compilers

#if VA_OPT_SUPPORTED  // Standardized in C++20
#define GET_ARG_COUNT(...)                                                 \
    INTERNAL_GET_ARG_COUNT_PRIVATE(                                        \
        unused __VA_OPT__(, ) __VA_ARGS__, 100, 99, 98, 97, 96, 95, 94,    \
        93, 92, 91, 90, 89, 88, 87, 86, 85, 84, 83, 82, 81, 80, 79, 78,    \
        77, 76, 75, 74, 73, 72, 71, 70, 69, 68, 67, 66, 65, 64, 63, 62,    \
        61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46,    \
        45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30,    \
        29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14,    \
        13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#elif defined(__GNUC__)  // Extension in GCC/Clang
#define GET_ARG_COUNT(...)                                                 \
    INTERNAL_GET_ARG_COUNT_PRIVATE(                                        \
        unused, ##__VA_ARGS__, 100, 99, 98, 97, 96, 95, 94, 93, 92, 91,    \
        90, 89, 88, 87, 86, 85, 84, 83, 82, 81, 80, 79, 78, 77, 76, 75,    \
        74, 73, 72, 71, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59,    \
        58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43,    \
        42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27,    \
        26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11,    \
        10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#else  // GET_ARG_COUNT() may return 1 here
#define GET_ARG_COUNT(...)                                                 \
    INTERNAL_GET_ARG_COUNT_PRIVATE(                                        \
        unused, __VA_ARGS__, 100, 99, 98, 97, 96, 95, 94, 93, 92, 91, 90,  \
        89, 88, 87, 86, 85, 84, 83, 82, 81, 80, 79, 78, 77, 76, 75, 74,    \
        73, 72, 71, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58,    \
        57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42,    \
        41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26,    \
        25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, \
        8, 7, 6, 5, 4, 3, 2, 1, 0)
#endif

#endif

#define INTERNAL_GET_ARG_COUNT_PRIVATE(                                    \
    e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15,  \
    e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29,  \
    e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43,  \
    e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57,  \
    e58, e59, e60, e61, e62, e63, e64, e65, e66, e67, e68, e69, e70, e71,  \
    e72, e73, e74, e75, e76, e77, e78, e79, e80, e81, e82, e83, e84, e85,  \
    e86, e87, e88, e89, e90, e91, e92, e93, e94, e95, e96, e97, e98, e99,  \
    e100, count, ...)                                                      \
    count

#define EXPAND(x) x // for msvc
#define REPEAT_0(func, i, arg)
#define REPEAT_1(func, i, arg)        func(i, arg)
#define REPEAT_2(func, i, arg, ...)   func(i, arg) EXPAND(REPEAT_1(func, i + 1, __VA_ARGS__))
#define REPEAT_3(func, i, arg, ...)   func(i, arg) EXPAND(REPEAT_2(func, i + 1, __VA_ARGS__))
#define REPEAT_4(func, i, arg, ...)   func(i, arg) EXPAND(REPEAT_3(func, i + 1, __VA_ARGS__))
#define REPEAT_5(func, i, arg, ...)   func(i, arg) EXPAND(REPEAT_4(func, i + 1, __VA_ARGS__))
#define REPEAT_6(func, i, arg, ...)   func(i, arg) EXPAND(REPEAT_5(func, i + 1, __VA_ARGS__))
#define REPEAT_7(func, i, arg, ...)   func(i, arg) EXPAND(REPEAT_6(func, i + 1, __VA_ARGS__))
#define REPEAT_8(func, i, arg, ...)   func(i, arg) EXPAND(REPEAT_7(func, i + 1, __VA_ARGS__))
#define REPEAT_9(func, i, arg, ...)   func(i, arg) EXPAND(REPEAT_8(func, i + 1, __VA_ARGS__))
#define REPEAT_10(func, i, arg, ...)  func(i, arg) EXPAND(REPEAT_9(func, i + 1, __VA_ARGS__))
#define REPEAT_11(func, i, arg, ...)  func(i, arg) EXPAND(REPEAT_10(func, i + 1, __VA_ARGS__))
#define REPEAT_12(func, i, arg, ...)  func(i, arg) EXPAND(REPEAT_11(func, i + 1, __VA_ARGS__))
#define REPEAT_13(func, i, arg, ...)  func(i, arg) EXPAND(REPEAT_12(func, i + 1, __VA_ARGS__))
#define REPEAT_14(func, i, arg, ...)  func(i, arg) EXPAND(REPEAT_13(func, i + 1, __VA_ARGS__))
#define REPEAT_15(func, i, arg, ...)  func(i, arg) EXPAND(REPEAT_14(func, i + 1, __VA_ARGS__))
#define REPEAT_16(func, i, arg, ...)  func(i, arg) EXPAND(REPEAT_15(func, i + 1, __VA_ARGS__))
#define REPEAT_17(func, i, arg, ...)  func(i, arg) EXPAND(REPEAT_16(func, i + 1, __VA_ARGS__))
#define REPEAT_18(func, i, arg, ...)  func(i, arg) EXPAND(REPEAT_17(func, i + 1, __VA_ARGS__))
#define REPEAT_19(func, i, arg, ...)  func(i, arg) EXPAND(REPEAT_18(func, i + 1, __VA_ARGS__))
#define REPEAT_20(func, i, arg, ...)  func(i, arg) EXPAND(REPEAT_19(func, i + 1, __VA_ARGS__))
#define REPEAT_21(func, i, arg, ...)  func(i, arg) EXPAND(REPEAT_20(func, i + 1, __VA_ARGS__))
#define REPEAT_22(func, i, arg, ...)  func(i, arg) EXPAND(REPEAT_21(func, i + 1, __VA_ARGS__))
#define REPEAT_23(func, i, arg, ...)  func(i, arg) EXPAND(REPEAT_22(func, i + 1, __VA_ARGS__))
#define REPEAT_24(func, i, arg, ...)  func(i, arg) EXPAND(REPEAT_23(func, i + 1, __VA_ARGS__))
#define REPEAT_25(func, i, arg, ...)  func(i, arg) EXPAND(REPEAT_24(func, i + 1, __VA_ARGS__))
#define REPEAT_26(func, i, arg, ...)  func(i, arg) EXPAND(REPEAT_25(func, i + 1, __VA_ARGS__))
#define REPEAT_27(func, i, arg, ...)  func(i, arg) EXPAND(REPEAT_26(func, i + 1, __VA_ARGS__))
#define REPEAT_28(func, i, arg, ...)  func(i, arg) EXPAND(REPEAT_27(func, i + 1, __VA_ARGS__))
#define REPEAT_29(func, i, arg, ...)  func(i, arg) EXPAND(REPEAT_28(func, i + 1, __VA_ARGS__))
#define REPEAT_30(func, i, arg, ...)  func(i, arg) EXPAND(REPEAT_29(func, i + 1, __VA_ARGS__))
#define REPEAT_31(func, i, arg, ...)  func(i, arg) EXPAND(REPEAT_30(func, i + 1, __VA_ARGS__))
#define REPEAT_32(func, i, arg, ...)  func(i, arg) EXPAND(REPEAT_31(func, i + 1, __VA_ARGS__))
#define REPEAT_33(func, i, arg, ...)  func(i, arg) EXPAND(REPEAT_32(func, i + 1, __VA_ARGS__))
#define REPEAT_34(func, i, arg, ...)  func(i, arg) EXPAND(REPEAT_33(func, i + 1, __VA_ARGS__))
#define REPEAT_35(func, i, arg, ...)  func(i, arg) EXPAND(REPEAT_34(func, i + 1, __VA_ARGS__))
#define REPEAT_36(func, i, arg, ...)  func(i, arg) EXPAND(REPEAT_35(func, i + 1, __VA_ARGS__))
#define REPEAT_37(func, i, arg, ...)  func(i, arg) EXPAND(REPEAT_36(func, i + 1, __VA_ARGS__))
#define REPEAT_38(func, i, arg, ...)  func(i, arg) EXPAND(REPEAT_37(func, i + 1, __VA_ARGS__))
#define REPEAT_39(func, i, arg, ...)  func(i, arg) EXPAND(REPEAT_38(func, i + 1, __VA_ARGS__))
#define REPEAT_40(func, i, arg, ...)  func(i, arg) EXPAND(REPEAT_39(func, i + 1, __VA_ARGS__))
#define REPEAT_41(func, i, arg, ...)  func(i, arg) EXPAND(REPEAT_40(func, i + 1, __VA_ARGS__))
#define REPEAT_42(func, i, arg, ...)  func(i, arg) EXPAND(REPEAT_41(func, i + 1, __VA_ARGS__))
#define REPEAT_43(func, i, arg, ...)  func(i, arg) EXPAND(REPEAT_42(func, i + 1, __VA_ARGS__))
#define REPEAT_44(func, i, arg, ...)  func(i, arg) EXPAND(REPEAT_43(func, i + 1, __VA_ARGS__))
#define REPEAT_45(func, i, arg, ...)  func(i, arg) EXPAND(REPEAT_44(func, i + 1, __VA_ARGS__))
#define REPEAT_46(func, i, arg, ...)  func(i, arg) EXPAND(REPEAT_45(func, i + 1, __VA_ARGS__))
#define REPEAT_47(func, i, arg, ...)  func(i, arg) EXPAND(REPEAT_46(func, i + 1, __VA_ARGS__))
#define REPEAT_48(func, i, arg, ...)  func(i, arg) EXPAND(REPEAT_47(func, i + 1, __VA_ARGS__))
#define REPEAT_49(func, i, arg, ...)  func(i, arg) EXPAND(REPEAT_48(func, i + 1, __VA_ARGS__))
#define REPEAT_50(func, i, arg, ...)  func(i, arg) EXPAND(REPEAT_49(func, i + 1, __VA_ARGS__))
#define REPEAT_51(func, i, arg, ...)  func(i, arg) EXPAND(REPEAT_50(func, i + 1, __VA_ARGS__))
#define REPEAT_52(func, i, arg, ...)  func(i, arg) EXPAND(REPEAT_51(func, i + 1, __VA_ARGS__))
#define REPEAT_53(func, i, arg, ...)  func(i, arg) EXPAND(REPEAT_52(func, i + 1, __VA_ARGS__))
#define REPEAT_54(func, i, arg, ...)  func(i, arg) EXPAND(REPEAT_53(func, i + 1, __VA_ARGS__))
#define REPEAT_55(func, i, arg, ...)  func(i, arg) EXPAND(REPEAT_54(func, i + 1, __VA_ARGS__))
#define REPEAT_56(func, i, arg, ...)  func(i, arg) EXPAND(REPEAT_55(func, i + 1, __VA_ARGS__))
#define REPEAT_57(func, i, arg, ...)  func(i, arg) EXPAND(REPEAT_56(func, i + 1, __VA_ARGS__))
#define REPEAT_58(func, i, arg, ...)  func(i, arg) EXPAND(REPEAT_57(func, i + 1, __VA_ARGS__))
#define REPEAT_59(func, i, arg, ...)  func(i, arg) EXPAND(REPEAT_58(func, i + 1, __VA_ARGS__))
#define REPEAT_60(func, i, arg, ...)  func(i, arg) EXPAND(REPEAT_59(func, i + 1, __VA_ARGS__))
#define REPEAT_61(func, i, arg, ...)  func(i, arg) EXPAND(REPEAT_60(func, i + 1, __VA_ARGS__))
#define REPEAT_62(func, i, arg, ...)  func(i, arg) EXPAND(REPEAT_61(func, i + 1, __VA_ARGS__))
#define REPEAT_63(func, i, arg, ...)  func(i, arg) EXPAND(REPEAT_62(func, i + 1, __VA_ARGS__))
#define REPEAT_64(func, i, arg, ...)  func(i, arg) EXPAND(REPEAT_63(func, i + 1, __VA_ARGS__))

#define STR(x) #x
#define CONCATE(x, y) x ## y
#define STRING(x) EXPAND(STR(x))
#define PARE(...) __VA_ARGS__
#define EAT(...)
#define PAIR(x) PARE x // PAIR((int) x) => PARE(int) x => int x
#define STRIP(x) EXPAND(EAT x) // STRIP((int) x) => EAT(int) x => x
#define PASTE(x, y) CONCATE(x, y)


#define FIELD_EACH(i, arg)                     \
    PAIR(arg);                                 \
    template <typename T>                      \
    struct FIELD<T, i> {                       \
        T& obj;                                \
        FIELD(T& obj): obj(obj) {}             \
        auto value() -> decltype(auto) {       \
            return (obj.STRIP(arg));           \
        }                                      \
        static constexpr const char* name() {  \
            return EXPAND(STRING(STRIP(arg))); \
        }                                      \
    };                                         \



#define DEFINE_STRUCT(st, ...)                                                          \
    struct st {                                                                         \
        template <typename, size_t> struct FIELD;                                       \
        static constexpr size_t _field_count_ = GET_ARG_COUNT(__VA_ARGS__);             \
        static constexpr decltype(#st) _struct_name_ = #st;                             \
        bool serialize(QJsonObject& json)                                                \
        {                                                                               \
            return struct_serialize::Serializ<st>(*this, json);                         \
        }                                                                               \
        bool serialize(QString& json)                                                    \
        {                                                                               \
            return struct_serialize::Serializ<st>(*this, json);                         \
        }                                                                               \
        bool deserialize(QJsonObject& json)                                              \
        {                                                                               \
            return struct_serialize::Deserializ<st>(json, *this);                       \
        }                                                                               \
        EXPAND(PASTE(REPEAT_, GET_ARG_COUNT(__VA_ARGS__)) (FIELD_EACH, 0, __VA_ARGS__)) \
    }                                                                                   \

#if __cplusplus >= 201703L

template<typename T, typename F, size_t... Is>
inline constexpr void forEach(T&& obj, F&& f, std::index_sequence<Is...>) {
    using TDECAY = std::decay_t<T>;
    (void(f(typename TDECAY::template FIELD<T, Is>(std::forward<T>(obj)).name(),
        typename TDECAY::template FIELD<T, Is>(std::forward<T>(obj)).value())), ...);
}

template<typename T, typename F>
inline constexpr void forEach(T&& obj, F&& f) {
    forEach(std::forward<T>(obj),
        std::forward<F>(f),
        std::make_index_sequence<std::decay_t<T>::_field_count_>{});
}

#else


template<typename T, typename F, size_t... Is>
inline void forEach(T&& obj, F&& f, std::index_sequence<Is...>) {
    using TDECAY = typename std::decay<T>::type;
    (void)std::initializer_list<int>{(f(typename TDECAY::template FIELD<T, Is>(std::forward<T>(obj)).name(),
                                        typename TDECAY::template FIELD<T, Is>(std::forward<T>(obj)).value()), 0)...};
}


template<typename T, typename F>
inline void forEach(T&& obj, F&& f) 
{
    forEach(std::forward<T>(obj),
            std::forward<F>(f),
            std::make_index_sequence<std::decay_t<T>::_field_count_>{});
}

#endif




namespace struct_serialize
{

template<bool support>
struct IsSupport {
    static constexpr bool isSupport = support;
};

template<typename T, typename = void>
struct IsReflected : std::false_type { };


template<typename...>
using Void = void;

template<typename T>
struct IsReflected<T, std::void_t<decltype(&T::_field_count_)>>
    : std::true_type { };

template<typename T>
constexpr static bool IsReflected_v = IsReflected<T>::value;


template<typename T, typename = void>
struct IsString : std::false_type { };

template<>
struct IsString<std::string>
    : std::true_type { };

template<>
struct IsString<QString>
    : std::true_type { };

template<typename T>
constexpr static bool IsString_v = IsString<T>::value;



template<typename T, typename = void>
struct BaseSerializeTraits : IsSupport<false> {};


template<typename T>
struct BaseSerializeTraits<T, typename std::enable_if<std::is_arithmetic_v<T>>::type> 
    : IsSupport<true> 
{
    static T Dump(T& obj, bool &ret, const char* fieldName) 
    {
        ret = true;
        return obj;
    }

    static bool Load(QJsonValueRef json, bool& obj)
    {
        obj = json.toBool();
        return true;
    }

    static bool Load(QJsonObject &json, bool &obj, const char* fieldName)
    {
        if (!json.contains(fieldName))
            return false;
        obj = json[fieldName].toBool();
        return true;
    }

    static bool Load(QJsonValueRef json, int& obj)
    {
        obj = json.toInt();
        return true;
    }

    static bool Load(QJsonObject& json, int& obj, const char* fieldName)
    {
        if (!json.contains(fieldName))
            return false;
        obj = json[fieldName].toInt();
        return true;
    }

    static bool Load(QJsonValueRef json, double& obj)
    {
        obj = json.toDouble();
        return true;
    }

    static bool Load(QJsonObject& json, double& obj, const char* fieldName)
    {
        if (!json.contains(fieldName))
            return false;
        obj = json[fieldName].toDouble();
        return true;
    }
};


template<typename T>
struct BaseSerializeTraits<T, typename std::enable_if<IsString_v<T>>::type>
    : IsSupport<true>
{
    static QString Dump(std::string& obj, bool& ret, const char* fieldName)
    {
        ret = true;
        return QString::fromStdString(obj);
    }

    static QString Dump(QString& obj, bool& ret, const char* fieldName)
    {
        ret = true;
        return obj;
    }

    static bool Load(QJsonValueRef json, QString& obj)
    {
        obj = json.toString();
        return true;
    }

    static bool Load(QJsonObject& json, QString& obj, const char* fieldName)
    {
        if (!json.contains(fieldName))
            return false;
        obj = json[fieldName].toString();
        return true;
    }

    static bool Load(QJsonValueRef json, std::string& obj)
    {
        obj = json.toString().toStdString();
        return true;
    }

    static bool Load(QJsonObject& json, std::string& obj, const char* fieldName)
    {
        if (!json.contains(fieldName))
            return false;
        obj = json[fieldName].toString().toStdString();
        return true;
    }
};



template<typename T, typename = void>
struct GeneralSerializeTraits {};


template<typename T>
struct GeneralSerializeTraits <T, typename std::enable_if<BaseSerializeTraits<T>::isSupport>::type>
    :BaseSerializeTraits<T>
{};


template<typename Container>
struct ContainerSerializeTraits 
{
    static QJsonArray Dump(Container& obj, bool& ret, const char* fieldName)
    {
        QJsonArray array;
        for (auto &&elem : obj)
        {
            typename std::decay<decltype(elem)>::type e;
            bool res = false;
            array.push_back(GeneralSerializeTraits< std::decay_t<decltype(elem)>>::Dump(elem,res,""));
            if (!res)
            {
                ret = false;
                return array;
            }
        }
        ret = true;
        return array;
    }

    static bool Load(QJsonValueRef  json, Container& obj)
    {
        QJsonArray array;
        if (json.isArray())
        {
            array = json.toArray();
        }
        else
            return false;

        using ElementType = typename Container::value_type;
        for (auto&& elem : array)
        {
            ElementType e;
            bool res = false;
            res = GeneralSerializeTraits<std::decay_t<decltype(e)>>::Load(elem, e);
            if (!res)
            {
                return false;
            }
            obj.push_back(e);
        }
        return true;
    }

    static bool Load(QJsonObject &json, Container &obj, const char * fieldName)
    {
        QJsonArray array;
        if (json.contains(fieldName))
        {
            array = json[fieldName].toArray();
        }
        else
        {
            return false;
        }
        using ElementType = typename Container::value_type;
        for (int i=0;i<array.size();i++)
        {
            ElementType e;
            bool res = false;
            res = GeneralSerializeTraits<std::decay_t<decltype(e)>>::Load(array[i], e);
            
            if (!res)
            {
                return false;
            }
            obj.push_back(e);
        }
        return true;
    }
};




template<typename T>
struct GeneralSerializeTraits <std::vector<T>> :ContainerSerializeTraits<std::vector<T>> {};




template<typename T>
struct GeneralSerializeTraits<T, typename std::enable_if<IsReflected_v<T>>::type>
{
    static QJsonObject Dump(T &obj, bool &ret, const char* fieldName)
    {
        std::bitset<T::_field_count_> results;
        int index = 0;
        QJsonObject json;
        forEach(obj,
            [&](auto&& _fieldName, auto&& value) 
            { 
                bool res = false;
                json[_fieldName] = GeneralSerializeTraits<std::decay_t<decltype(value)>>::Dump(value, res, _fieldName);
                results[index++] = res;
            }
        );
        ret = results.all();
        return json;
    }

    static bool Load(QJsonObject &json, T &obj, const char * fieldName) 
    {
        QJsonObject item;
        if (json.contains(fieldName))
        {
            item = json[fieldName].toObject();
        }
        else
        {
            item = json;
        }

        if (json.size() != T::_field_count_)
            return false;

        std::bitset<T::_field_count_> results;
        int index = 0;
        forEach(obj,
            [&](auto&& _fieldName, auto&& value)
            {
                if (!item.contains(_fieldName))
                {
                    results[index++] = false;
                    return;
                }
                results[index++] = GeneralSerializeTraits<std::decay_t<decltype(value)>>::Load(item, value, _fieldName);
            }
        );

        return results.all();
    }
};



template <typename T>
bool Serializ(T &obj, QJsonObject &json)
{
    if (IsReflected_v<T>)
    {
        bool ret = false;
        json = GeneralSerializeTraits<T>::Dump(obj,ret,"");
        return ret;
    }
    return false;
}

template <typename T>
bool Serializ(T& obj, QString& json)
{
    if (IsReflected_v<T>)
    {
        bool ret = false;
        QJsonObject out = GeneralSerializeTraits<T>::Dump(obj, ret, "");
        json = QJsonDocument(out).toJson(QJsonDocument::Indented);
        return ret;
    }
    return false;
}

template<typename T>
bool Deserializ(QJsonObject &json, T &obj)
{
    if (IsReflected_v<T>)
    {
        return GeneralSerializeTraits<T>::Load(json, obj, "");   
    }
    return false;
}


} // ! namespace struct_serialize

#endif // !SERIALIZER_H
