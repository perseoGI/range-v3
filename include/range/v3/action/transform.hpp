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

#ifndef RANGES_V3_ACTION_TRANSFORM_HPP
#define RANGES_V3_ACTION_TRANSFORM_HPP

#include <functional>
#include <range/v3/range_fwd.hpp>
#include <range/v3/action/action.hpp>
#include <range/v3/algorithm/transform.hpp>
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
                template(typename Rng, typename F, typename P = identity)
                (concept TransformActionConcept)(Rng, F, P),
                    InputRange<Rng> &&
                    Transformable1<iterator_t<Rng>, iterator_t<Rng>, F, P>
            );

            struct transform_fn
            {
            private:
                friend action_access;
                template<typename F, typename P = identity>
                static auto CPP_fun(bind)(transform_fn transform, F fun, P proj = P{})(
                    requires not Range<F>)
                {
                    return std::bind(transform, std::placeholders::_1, protect(std::move(fun)),
                        protect(std::move(proj)));
                }
            public:
                CPP_template(typename Rng, typename F, typename P = identity)(
                    requires TransformActionConcept<Rng, F, P>)
                Rng operator()(Rng &&rng, F fun, P proj = P{}) const
                {
                    ranges::transform(rng, begin(rng), std::move(fun), std::move(proj));
                    return static_cast<Rng &&>(rng);
                }

            #ifndef RANGES_DOXYGEN_INVOKED
                CPP_template(typename Rng, typename F, typename P = identity)(
                    requires not TransformActionConcept<Rng, F, P>)
                void operator()(Rng &&, F &&, P && = P{}) const
                {
                    CPP_assert_msg(InputRange<Rng>,
                        "The object on which action::transform operates must be a model of the "
                        "InputRange concept.");
                    using I = iterator_t<Rng>;
                    CPP_assert_msg(IndirectInvocable<P, I>,
                        "The projection function must accept objects of the iterator's value type, "
                        "reference type, and common reference type.");
                    CPP_assert_msg(IndirectInvocable<F, projected<I, P>>,
                        "The function argument to action::transform must be callable with "
                        "the result of the projection argument, or with objects of the range's "
                        "common reference type if no projection is specified.");
                    CPP_assert_msg(Writable<iterator_t<Rng>,
                            invoke_result_t<F&,
                                invoke_result_t<P&, range_common_reference_t<Rng>>>>,
                        "The result type of the function passed to action::transform must "
                        "be writable back into the source range.");
                }
            #endif
            };

            /// \ingroup group-actions
            /// \relates transform_fn
            /// \sa action
            RANGES_INLINE_VARIABLE(action<transform_fn>, transform)
        }
        /// @}
    }
}

#endif
