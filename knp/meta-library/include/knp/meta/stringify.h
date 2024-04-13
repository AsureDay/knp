/**
 * @file stringify.h
 * @brief Introspection routines.
 * @author Artiom N.
 * @date 10.05.2023
 */

#pragma once

#include <string>
#include <tuple>
#include <vector>

#include <boost/mp11.hpp>
#include <boost/preprocessor.hpp>


/**
 * @brief Metaprogramming library namespace.
 */
namespace knp::meta
{

/**
 * @brief Template used to determine if two arbitrary types are not the same at the compile stage.
 * @details `mp_neq` takes the value of `true` if the types are not the same, otherwise `mp_neq` takes the value of
 * `false`.
 * @tparam T1 first arbitrary type to compare.
 * @tparam T2 second arbitrary type to compare.
 */
template <class T1, class T2>
using mp_neq = boost::mp11::mp_bool<T1::value != T2::value>;

/**
 * @brief List of types from the global type list that are used in the local type list.
 * @tparam AllList global list of types.
 * @tparam L local list of types (subset of `AllList`).
 */
template <class AllList, class L>
using mp_flt = boost::mp11::mp_filter_q<boost::mp11::mp_bind_front<mp_neq, boost::mp11::mp_size<AllList>>, L>;

/**
 * @brief Index array for types from the global type list that are used in the local type list.
 * @tparam AllList global list of types.
 * @tparam L local list of types (subset of `AllList`).
 */
template <class AllList, class L>
using mp_supported_indexes =
    mp_flt<AllList, boost::mp11::mp_transform_q<boost::mp11::mp_bind_front<boost::mp11::mp_find, AllList>, L>>;

/**
 * @brief Internal macro.
 */
#define KNP_MAKE_TUPLE_INTERNAL(n, tuple_type, tuple_elem) (BOOST_PP_STRINGIZE(tuple_elem))

/**
 * @brief Make tuple from the passed elements.
 */
#define KNP_MAKE_TUPLE(tuple_elems) \
    ::std::make_tuple(              \
        BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_FOR_EACH(KNP_MAKE_TUPLE_INTERNAL, "", BOOST_PP_VARIADIC_TO_SEQ(tuple_elems))));

/**
 * @brief Get supported type indexes.
 * @tparam AllTypes global list of types.
 * @tparam SupportedTypes local list of types.
 * @return vector with indexes of supported types.
 */
template <typename AllTypes, typename SupportedTypes>
std::vector<size_t> get_supported_type_indexes()
{
    std::vector<size_t> result;
    result.reserve(boost::mp11::mp_size<SupportedTypes>::value);

    boost::mp11::mp_for_each<knp::meta::mp_supported_indexes<SupportedTypes, AllTypes>>([&result](auto i)
                                                                                        { result.push_back(i); });

    return result;
}

/**
 * @brief Get names of supported object types.
 * @param type_names container (usually a tuple) of type names.
 * @tparam AllTypes global list of types.
 * @tparam SupportedTypes local list of types.
 * @tparam NamesType container type (automatically derived from `type_names` if not specified).
 * @return vector of type names.
 */
template <typename AllTypes, typename SupportedTypes, typename NamesType>
std::vector<std::string> get_supported_type_names(const NamesType &type_names)
{
    std::vector<std::string> result;
    result.reserve(boost::mp11::mp_size<SupportedTypes>::value);

    boost::mp11::mp_for_each<knp::meta::mp_supported_indexes<SupportedTypes, AllTypes>>(
        [&type_names, &result](auto i) { result.push_back(std::get<i>(type_names)); });

    return result;
}

}  // namespace knp::meta
