#ifndef ENTT_CORE_FAMILY_HPP
#define ENTT_CORE_FAMILY_HPP


#include<algorithm>
#include<cassert>
#include<cstddef>
#include<cstdlib>
#include<tuple>
#include<type_traits>
#include<utility>
#include<vector>

namespace entt {

namespace detail
{

/**
 * @brief Unique identifier for T
 *
 * Utility class template that can be used to assign unique identifiers to types
 * at runtime. Use different specializations to create separate sets of
 * identifiers.
 */
class type_id_t
{
    using sig = type_id_t();

    sig* id;
    type_id_t(sig* id) : id{id} {}

public:
    template<typename ...T>
    friend type_id_t type_id();

    bool operator==(type_id_t o) const { return id == o.id; }
    bool operator!=(type_id_t o) const { return id != o.id; }

    // https://codereview.stackexchange.com/a/45079
    size_t as_address() const { return reinterpret_cast<size_t>(id); }
};

template<typename ...T>
type_id_t type_id() { return &type_id<T...>; }

/**
 * @brief Container for type IDs
 *
 * Runtime container for type identifiers. Undefined behavior to index past the total number of
 * types stored internally.
 */
class TypeIDs
{
  std::vector<type_id_t> types;
public:
  TypeIDs() = default;
  TypeIDs(TypeIDs const&) = delete;
  TypeIDs& operator=(TypeIDs const&) = delete;

  TypeIDs(TypeIDs &&) = default;
  TypeIDs& operator=(TypeIDs &&) = default;


  void add(type_id_t const& t)
  {
    types.emplace_back(t);
  }

  bool contains(type_id_t const& id) const
  {
    for (size_t i = 0; i < types.size(); ++i) {
      if (id == types[i]) {
        return true;
      }
    }
    return false;
  }

  size_t operator[](type_id_t const id) const
  {
    assert(contains(id));

    for (size_t i = 0; i < types.size(); ++i) {
      if (id == types[i]) {
        assert(types[i].as_address() == id.as_address());
        return types[i].as_address();
      }
    }

    // Should this just be undefined behavior, or abort() ?
    std::abort();
    return 0;
  }
};

/**
 * @brief generates a type id for a parameter pack
 *
 * Utility function that converts a parameter pack into a type_id_t instance.
 */
template<typename... T>
type_id_t
make_ids() { return { type_id<T...>() }; }

}

/**
 * @brief Dynamic identifier generator.
 *
 * Utility class template that can be used to assign unique identifiers to types
 * at runtime. Use different specializations to create separate sets of
 * identifiers.
 */
template<typename...COMPONENTS>
class Family {

    mutable detail::TypeIDs type_container_;

    template<typename ...args>
    std::size_t identifier() const noexcept {
      auto const id = detail::make_ids<args...>();

      if (type_container_.contains(id)) {
        return type_container_[id];
      }

      type_container_.add(id);
      assert(type_container_.contains(id));

      return type_container_[id];
    }

    template<typename ...args>
    std::size_t family() const {
      const std::size_t value = identifier<args...>();
      return value;
    }

public:
    Family() = default;

    /*! @brief Unsigned integer type. */
    using family_type = std::size_t;

    /**
     * @brief Returns an unique identifier for the given type.
     * @return Statically generated unique identifier for the given type.
     */
    template<typename... Type>
    family_type type() const {
        return family<std::decay_t<Type>...>();
    }
};


}


#endif // ENTT_CORE_FAMILY_HPP
