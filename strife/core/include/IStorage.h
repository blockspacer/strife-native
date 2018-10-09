#ifndef ISTORAGE_H
#define ISTORAGE_H

#include <utility>

#include "IIterator.h"
#include "Component.h"
#include "Data.h"
#include "Entity.h"

namespace Strife {
    namespace Core {

        class IStorage {

        public:

            class Iterator {

            public:

                Iterator(IIterator<std::pair<const Entity, Component* const>>* const iterator);
                ~Iterator();

                Iterator& operator++();
                Iterator& operator++(int);
                std::pair<const Entity, Component* const> operator*();
                bool operator==(const Iterator& iterator) const;
                bool operator!=(const Iterator& iterator) const;

            protected:

                IIterator<std::pair<const Entity, Component* const>>* const iterator_;

            };

            template <class S>
            static void AssertBase();

            IStorage(Scene* const scene);;
            virtual ~IStorage() = default;

            virtual const Data serialize() const = 0;
            virtual void deserialize(const Data data) = 0;

            virtual Component* const add(const Entity entity) = 0;
            virtual Component* const add(const boost::uuids::uuid id, const Entity entity) = 0;
            virtual void remove(const Entity entity) = 0;
            virtual Component* const get(const Entity entity) const = 0;

            virtual Iterator begin() = 0;
            virtual Iterator end() = 0;

        protected:

            Scene* const scene_;

        };

        template <class S>
        void IStorage::AssertBase() {
            static_assert(
                std::is_base_of<IStorage, S>::value,
                "Type not derived from IStorage"
            );
        };

    }
}
#endif // ISTORAGE_H
