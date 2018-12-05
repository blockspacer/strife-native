#ifndef ISYSTEM_H
#define ISYSTEM_H

#include "Dispatcher.h"

namespace Strife {
    namespace Core {
        
        class ISystem {
            
        public:
        
            template <class S>
            static void AssertBase();
            
            ISystem(Scene& scene, Dispatcher& dispatcher);
            virtual ~ISystem() = default;
            
        protected:
        
            Scene& scene_;
            Dispatcher& dispatcher_;
            
        };
        
        template <class S>
		void ISystem::AssertBase() {
			static_assert(std::is_base_of<ISystem, S>::value, "Type not derived from ISystem");
		}
        
    }
}

#endif