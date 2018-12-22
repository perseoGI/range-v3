/// \file
// Range v3 library
//
//  Copyright Eric Niebler 2013-present
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/ericniebler/range-v3
//

#ifndef RANGES_V3_ACTION_SORT_HPP
#define RANGES_V3_ACTION_SORT_HPP

#include <functional>
#include <range/v3/range_fwd.hpp>
#include <range/v3/action/action.hpp>
#include <range/v3/algorithm/sort.hpp>
#include <range/v3/utility/functional.hpp>
#include <range/v3/utility/iterator_concepts.hpp>
#include <range/v3/utility/iterator_traits.hpp>
#include <range/v3/utility/static_const.hpp>

namespace ranges
{
    inline namespace v3
    {
        /// \addtogroup group-actions
        /// @{
        namespace action
        {
            CPP_def
            (
                template(typename Rng, typename C = less, typename P = identity)
                (concept SortActionConcept)(Rng, C, P),
                    ForwardRange<Rng> && Sortable<iterator_t<Rng>, C, P>
            );

            struct sort_fn
            {
            private:
                friend action_access;
                template<typename C, typename P = identity>
                static auto CPP_fun(bind)(sort_fn sort, C pred, P proj = P{})(
                    requires not Range<C>)
                {
                    return std::bind(sort, std::placeholders::_1, protect(std::move(pred)),
                        protect(std::move(proj)));
                }

            public:
                CPP_template(typename Rng, typename C = less, typename P = identity)(
                    requires SortActionConcept<Rng, C, P>)
                Rng operator()(Rng &&rng, C pred = C{}, P proj = P{}) const
                {
                    ranges::sort(rng, std::move(pred), std::move(proj));
                    return static_cast<Rng &&>(rng);
                }

            #ifndef RANGES_DOXYGEN_INVOKED
                CPP_template(typename Rng, typename C = less, typename P = identity)(
                    requires not SortActionConcept<Rng, C, P>)
                void operator()(Rng &&, C && = C{}, P && = P{}) const
                {
                    CPP_assert_msg(ForwardRange<Rng>,
                        "The object on which action::sort operates must be a model of the "
                        "ForwardRange concept.");
                    using I = iterator_t<Rng>;
                    CPP_assert_msg(IndirectInvocable<P, I>,
                        "The projection function must accept objects of the iterator's value type, "
                        "reference type, and common reference type.");
                    CPP_assert_msg(IndirectRelation<C, projected<I, P>>,
                        "The comparator passed to action::sort must accept objects returned "
                        "by the projection function, or of the range's value type if no projection "
                        "is specified.");
                    CPP_assert_msg(Permutable<I>,
                        "The iterator type of the range passed to action::sort must allow its "
                        "elements to be permuted; that is, the values must be movable and the "
                        "iterator must be mutable.");
                }
            #endif
            };

            /// \ingroup group-actions
            /// \relates sort_fn
            /// \sa `action`
            RANGES_INLINE_VARIABLE(action<sort_fn>, sort)
        }
        /// @}
    }
}

#endif
