#include <iostream>
#include <optional>
#include <boost/uuid/uuid.hpp>
#include "Component.h"
#include "Context.h"
#include "Data.h"
#include "Entity.h"
#include "Scene.h"
#include "System.h"

using namespace Strife::Core;
using namespace std;
using boost::uuids::uuid;

namespace Strife {
	namespace Core {
	
        void to_json(Data& data, const optional<Entity>& optional) {
			if (optional) {
				data = optional.value().id;
			} else {
				data = nullptr;
			}
	    }
	    
	    void from_json(const Data& data, optional<Entity>& optional) {
	        optional = std::nullopt;
	    }
	
	}
}

class OtherComponent : public Component {
    
    using Component::Component;
    
public:

	static const string Identifier;
	
	static void Initialize(System<OtherComponent>& system) {}
    
	const Data serialize() const {
	    return Data();
	}
	
    void deserialize(Context context) {}
    
};

const string OtherComponent::Identifier = "Other";

class HierarchyComponent : public Component {
    
    using Component::Component;
    
public:

	static const string Identifier;
    
    optional<Entity> parent;
    string value;
    
	const Data serialize() const {
	    Data data;
	    data["parent"] = parent;
	    return data;
	}
	
    void deserialize(Context context) {
        parent = context.get<optional<Entity>>("parent");
    }
    
};

const string HierarchyComponent::Identifier = "Hierarchy";

int main() {
	
	Scene* s = new Scene();
	s->components.initialize<HierarchyComponent>();
	s->components.initialize<OtherComponent>();
	s->deserialize("{\"components\":{\"Hierarchy\":{\"60a7adcb-8f76-438c-b95b-150f00507f41\":{\"parent\":\"e3140528-624b-4529-991a-423b03ed69a2\",\"value\":\"0\"},\"e3140528-624b-4529-991a-423b03ed69a2\":{\"parent\":null,\"value\":\"1\"}}}}"_json);
	s->deserialize("{\"components\":{\"Hierarchy\":{\"60a7adcb-8f76-438c-b95b-150f00507f41\":{\"parent\":\"e3140528-624b-4529-991a-423b03ed69a2\",\"value\":\"0\"},\"e3140528-624b-4529-991a-423b03ed69a2\":{\"parent\":null,\"value\":\"1\"}}}}"_json);
	cout << s->serialize() << endl;
	
	return 0;
	
}
