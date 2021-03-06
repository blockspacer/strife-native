#ifndef MESSENGER_H
#define MESSENGER_H

#include <functional>
#include <map>
#include <typeindex>
#include <vector>
#include "Message.h"

namespace Strife {
	namespace Core {

		class Messenger {
			
		public:

			template <class M>
			using Callback = std::function<void(const M&)>;
			
			template <class M>
			class Binding {
			public:
				
				Binding(Callback<M> callback)
					: callback_(callback) {}
					
				void operator()(const Message& message) {
					callback_(static_cast<const M&>(message));
				}
				
			private:
			
				Callback<M> callback_;
				
			};
		
		    void emit(const Message& message);
			void emit(std::type_index type, const Message& message);

			template <class M>
			void on(Callback<M> callback) {
			    Message::AssertBase<M>();
				const std::type_index type = std::type_index(typeid(M));
				auto& callbacks = callbacks_[type];
				Binding<M> binding(callback);
				callbacks.push_back(binding);
			}
			
			template <class M>
			void emit(const Message& message) {
			    Message::AssertBase<M>();
				const std::type_index type = std::type_index(typeid(M));
				emit(type, message);
			}

		private:
		
			std::map<const std::type_index, std::vector<Callback<Message>>> callbacks_;
			
		};

	}  // namespace Core
}  // namespace Strife

#endif
