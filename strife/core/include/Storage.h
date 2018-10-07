#ifndef STORAGE_H
#define STORAGE_H

#include <functional>
#include <map>
#include <set>
#include <string>
#include <boost/uuid/uuid.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>
#include "Component.h"
#include "Data.h"
#include "Entity.h"

namespace Strife {
    namespace Core {

        class Scene;

        class IStorage {

        public:
        
            class Iterator {
            
            public:
                
                class CoolIt {
                    
                public:
                    
                    CoolIt() = default;
                    virtual ~CoolIt() = default;
                    
                    
                    virtual void increment() = 0;
                    
                    virtual std::pair<const Entity, Component* const> dereference() = 0;
                    
                    virtual bool equal(const CoolIt* iterator) const = 0;
                };
                
                Iterator(CoolIt* it) {
                    it_ = it;
                };
                virtual ~Iterator() {
                    delete it_;
                };
                
                Iterator& operator++() {
                    it_->increment();
                    return *this;
                };
                
                Iterator& operator++(int) {
                    it_->increment();
                    return *this;
                };
                
                virtual std::pair<const Entity, Component* const> operator*() {
                    return it_->dereference();
                };
                
                virtual bool operator==(const Iterator& iterator) const {
                    return it_->equal(iterator.it_);
                };
                
                virtual bool operator!=(const Iterator& iterator) const {
                    return !it_->equal(iterator.it_);
                };
                
            protected:
                
                CoolIt* it_ = nullptr;

                
            };

            template <class S>
            static void AssertBase();
            
            IStorage(Scene* const scene) :
                scene_(scene) {};
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

        template <class C>
        class Storage : public IStorage {
            
            class Iterator : public IStorage::Iterator::CoolIt {
                
            public:
            
                Iterator(typename std::map<Entity, C>::iterator iterator) :
                    iterator_(iterator) {};
                    
                ~Iterator() = default;
            
                void increment() {
                    iterator_++;
                }
                
                std::pair<const Entity, Component* const> dereference() {
                    return {iterator_->first, &iterator_->second};
                }
                
                bool equal(const CoolIt* iterator) const {
                    
                    return iterator_ == static_cast<const Iterator*>(iterator)->iterator_;
                }
                
            private:
            
                typename std::map<Entity, C>::iterator iterator_;
                
            };

        public:

            Storage(Scene* const scene) :
                IStorage(scene) {}

            const Data serialize() const {
                Data data;
                for (const auto& pairEntityToComponent : components_) {
                    const Entity entity = pairEntityToComponent.first;
                    const C component = pairEntityToComponent.second;
                    const std::string entityIdentifier = boost::lexical_cast<std::string>(entity.id);
                    const Data componentData = component.serialize();
                    data[entityIdentifier] = componentData;
                }
                return data;
            }

            void deserialize(const Data data) {
                for (Data::const_iterator iteratorEntityIdentifierToComponentData = data.begin(); iteratorEntityIdentifierToComponentData != data.end(); iteratorEntityIdentifierToComponentData++) {
                    const std::string entityIdentifier = iteratorEntityIdentifierToComponentData.key();
                    const Data componentData = iteratorEntityIdentifierToComponentData.value();
                    const boost::uuids::uuid entityId = boost::lexical_cast<boost::uuids::uuid>(entityIdentifier);
                    const Entity entity(entityId, scene_);
                    C* const component = add(entity);
                    component->deserialize(componentData);
                }
            }

            C* const add(const Entity entity) {
                return &components_.emplace(entity, entity).first->second;
            }

            C* const add(const boost::uuids::uuid id, const Entity entity) {
                return &components_.try_emplace(entity, id, entity).first->second;
            }

            void remove(const Entity entity) {
                components_.erase(entity);
            }

            C* const get(const Entity entity) const {
                return const_cast<C* const>(&components_.at(entity));
            }
            
            IStorage::Iterator begin() {
                return IStorage::Iterator(new Iterator(components_.begin()));
            }
            
            IStorage::Iterator end() {
                return IStorage::Iterator(new Iterator(components_.end()));
            }
            
        private:
        
            std::map<Entity, C> components_;

        };

    }
}

#endif
