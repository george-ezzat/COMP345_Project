#include "Orders.h"

#include <sstream>
#include <algorithm>

#include "..\\Player\\Player.h"
#include "..\\Map\\Territory.h"

using std::string;
using std::vector;
using std::ostream;
using std::ostringstream;

static std::string safeDeref(const std::string *p) {
    if (p) return *p;
    return std::string("(null)");
}

// Order (base)
Order::Order() {
    // allocate pointer members
    orderType = new std::string("Unknown");
    effect = new std::string("");
    executed = new bool(false);
    issuer = nullptr;
}

Order::Order(const std::string &type, Player *iss) {
    orderType = new std::string(type);
    effect = new std::string("");
    executed = new bool(false);
    issuer = iss;
}

// Copy constructor
Order::Order(const Order &other) {
    orderType = new std::string(*(other.orderType));
    effect = new std::string(*(other.effect));
    executed = new bool(*(other.executed));
    issuer = other.issuer; 
}

Order &Order::operator=(const Order &other) {
    if (this == &other) {
        return *this;
    }

    // free existing owned members
    delete orderType;
    delete effect;
    delete executed;

    orderType = new std::string(*(other.orderType));
    effect = new std::string(*(other.effect));
    executed = new bool(*(other.executed));
    issuer = other.issuer;
    return *this;
}

Order::~Order() {
    // free owned resources
    delete orderType;
    delete effect;
    delete executed;
}

// Stream insertion prints order type, execution status and effect
ostream &operator<<(ostream &os, const Order &o) {
    os << "Order(" << safeDeref(o.orderType) << ", executed=";
    if (o.executed && *(o.executed)) os << "yes";
    else os << "no";
    os << ")";
    if (o.effect && !o.effect->empty()) {
        os << " effect='" << *o.effect << "'";
    }
    return os;
}

// Deploy
Deploy::Deploy() : Order("Deploy") {
    armies = new int(0);
    territory = nullptr;
}

Deploy::Deploy(int a, Territory *t, Player *iss) : Order("Deploy", iss) {
    armies = new int(a);
    territory = t;
}

Deploy::Deploy(const Deploy &other) : Order(other) {
    armies = new int(*(other.armies));
    territory = other.territory;
    issuer = other.issuer;
}

Deploy &Deploy::operator=(const Deploy &other) {
    if (this == &other) return *this;
    Order::operator=(other); // assign base part

    // replace armies
    delete armies;
    armies = new int(*(other.armies));
    territory = other.territory;
    issuer = other.issuer;
    return *this;
}

Deploy::~Deploy() {
    delete armies;
}

bool Deploy::validate() {
    // territory pointer must exist and armies must be positive
    if (!territory) {
        *effect = "Invalid: no territory specified";
        return false;
    }
    if (!armies || *armies <= 0) {
        *effect = "Invalid: non-positive armies";
        return false;
    }
    return true;
}

void Deploy::execute() {
    if (!validate()) {
        *executed = false;
        return;
    }

    // Increase territory armies and set effect text
    int before = territory->getArmies();
    territory->setArmies(before + *armies);

    ostringstream ss;
    ss << "Deployed " << *armies << " to " << territory->getName()
       << " (" << before << " -> " << territory->getArmies() << ")";
    *effect = ss.str();
    *executed = true;
}

Order *Deploy::clone() const {
    return new Deploy(*this);
}

// Advance
Advance::Advance() : Order("Advance") {
    armies = new int(0);
    source = nullptr;
    destination = nullptr;
}
Advance::Advance(int a, Territory *src, Territory *dst, Player *iss) : Order("Advance", iss) {
    armies = new int(a);
    source = src;
    destination = dst;
}

Advance::Advance(const Advance &other) : Order(other) {
    armies = new int(*(other.armies));
    source = other.source;
    destination = other.destination;
    issuer = other.issuer;
}

Advance &Advance::operator=(const Advance &other) {
    if (this == &other) return *this;
    Order::operator=(other);
    delete armies;
    armies = new int(*(other.armies));
    source = other.source;
    destination = other.destination;
    issuer = other.issuer;
    return *this;
}

Advance::~Advance() {
    delete armies;
}

bool Advance::validate() {
    if (!source || !destination) {
        *effect = "Invalid: missing source or destination";
        return false;
    }
    if (!armies || *armies <= 0) {
        *effect = "Invalid: non-positive armies";
        return false;
    }
    return true;
}

void Advance::execute() {
    if (!validate()) {
        *executed = false;
        return;
    }

    int sourceBefore = source->getArmies();
    int moveCount = std::min(*armies, sourceBefore);

    source->setArmies(sourceBefore - moveCount);

    int destBefore = destination->getArmies();
    destination->setArmies(destBefore + moveCount);

    ostringstream ss;
    ss << "Advanced " << moveCount << " from " << source->getName()
       << " (" << sourceBefore << "->" << source->getArmies() << ") to "
       << destination->getName() << " (" << destBefore << "->" << destination->getArmies() << ")";
    *effect = ss.str();
    *executed = true;
}

Order *Advance::clone() const {
    return new Advance(*this);
}

// Bomb
Bomb::Bomb() : Order("Bomb") {
    target = nullptr;
}
Bomb::Bomb(Territory *t, Player *iss) : Order("Bomb", iss) {
    target = t;
}
Bomb::Bomb(const Bomb &other) : Order(other) {
    target = other.target;
    issuer = other.issuer;
}
Bomb &Bomb::operator=(const Bomb &other) {
    if (this == &other) return *this;
    Order::operator=(other);
    target = other.target;
    issuer = other.issuer;
    return *this;
}
Bomb::~Bomb() {
}

bool Bomb::validate() {
    if (!target) {
        *effect = "Invalid: no target";
        return false;
    }
    return true;
}

void Bomb::execute() {
    if (!validate()) {
        *executed = false;
        return;
    }
    int before = target->getArmies();
    int after = before / 2; // half the armies
    target->setArmies(after);

    ostringstream ss;
    ss << "Bombed " << target->getName() << " (" << before << "->" << after << ")";
    *effect = ss.str();
    *executed = true;
}

Order *Bomb::clone() const {
    return new Bomb(*this);
}

// Blockade
Blockade::Blockade() : Order("Blockade") {
    target = nullptr;
}
Blockade::Blockade(Territory *t, Player *iss) : Order("Blockade", iss) {
    target = t;
}
Blockade::Blockade(const Blockade &other) : Order(other) {
    target = other.target;
    issuer = other.issuer;
}
Blockade &Blockade::operator=(const Blockade &other) {
    if (this == &other) return *this;
    Order::operator=(other);
    target = other.target;
    issuer = other.issuer;
    return *this;
}
Blockade::~Blockade() {
}

bool Blockade::validate() {
    if (!target) {
        *effect = "Invalid: no target";
        return false;
    }
    if (!issuer) {
        *effect = "Invalid: no issuer";
        return false;
    }
    Player *owner = target->getOwner();
    if (owner != issuer) {
        *effect = "Invalid: issuer does not own target";
        return false;
    }
    return true;
}

void Blockade::execute() {
    if (!validate()) {
        *executed = false;
        return;
    }

    int before = target->getArmies();

    // remove territory from owner and make neutral
    Player *owner = target->getOwner();
    if (owner) {
        owner->removeTerritory(target);
    }
    target->setArmies(before * 3); // triple armies
    target->setOwner(nullptr); // neutral

    ostringstream ss;
    ss << "Blockaded " << target->getName() << " (" << before << "->" << target->getArmies() << ") and owner set to neutral";
    *effect = ss.str();
    *executed = true;
}

Order *Blockade::clone() const {
    return new Blockade(*this);
}

// Airlift
Airlift::Airlift() : Order("Airlift") {
    armies = new int(0);
    source = nullptr;
    destination = nullptr;
}
Airlift::Airlift(int a, Territory *src, Territory *dst, Player *iss) : Order("Airlift", iss) {
    armies = new int(a);
    source = src;
    destination = dst;
}
Airlift::Airlift(const Airlift &other) : Order(other) {
    armies = new int(*(other.armies));
    source = other.source;
    destination = other.destination;
    issuer = other.issuer;
}
Airlift &Airlift::operator=(const Airlift &other) {
    if (this == &other) return *this;
    Order::operator=(other);
    delete armies;
    armies = new int(*(other.armies));
    source = other.source;
    destination = other.destination;
    issuer = other.issuer;
    return *this;
}
Airlift::~Airlift() {
    delete armies;
}

bool Airlift::validate() {
    if (!source || !destination) {
        *effect = "Invalid: missing source/destination";
        return false;
    }
    if (!armies || *armies <= 0) {
        *effect = "Invalid: non-positive armies";
        return false;
    }
    return true;
}

void Airlift::execute() {
    if (!validate()) {
        *executed = false;
        return;
    }

    int srcBefore = source->getArmies();
    int moveCount = std::min(*armies, srcBefore);
    source->setArmies(srcBefore - moveCount);

    int destBefore = destination->getArmies();
    destination->setArmies(destBefore + moveCount);

    ostringstream ss;
    ss << "Airlifted " << moveCount << " from " << source->getName()
       << " (" << srcBefore << "->" << source->getArmies() << ") to "
       << destination->getName() << " (" << destBefore << "->" << destination->getArmies() << ")";
    *effect = ss.str();
    *executed = true;
}

Order *Airlift::clone() const {
    return new Airlift(*this);
}

// Negotiate 
Negotiate::Negotiate() : Order("Negotiate") {
    targetPlayer = nullptr;
}
Negotiate::Negotiate(Player *p, Player *iss) : Order("Negotiate", iss) {
    targetPlayer = p;
    issuer = iss;
}
Negotiate::Negotiate(const Negotiate &other) : Order(other) {
    targetPlayer = other.targetPlayer;
}
Negotiate &Negotiate::operator=(const Negotiate &other) {
    if (this == &other) return *this;
    Order::operator=(other);
    targetPlayer = other.targetPlayer;
    issuer = other.issuer;
    return *this;
}
Negotiate::~Negotiate() {
}

bool Negotiate::validate() {
    if (!targetPlayer) {
        *effect = "Invalid: no player";
        return false;
    }
    return true;
}

void Negotiate::execute() {
    if (!validate()) {
        *executed = false;
        return;
    }

    ostringstream ss;
    ss << "Negotiated peace with " << targetPlayer->getName();
    *effect = ss.str();
    *executed = true;
}

Order *Negotiate::clone() const {
    return new Negotiate(*this);
}

// OrdersList
OrdersList::OrdersList() {
    orders = new vector<Order *>();
}

OrdersList::OrdersList(const OrdersList &other) {
    orders = new vector<Order *>();
    for (Order *o : *(other.orders)) {
        // clone each order
        orders->push_back(o->clone());
    }
}

OrdersList &OrdersList::operator=(const OrdersList &other) {
    if (this == &other) {
        return *this;
    }

    // delete existing owned orders
    for (Order *o : *orders) {
        delete o;
    }
    orders->clear();

    // deep-copy from other
    for (Order *o : *(other.orders)) {
        orders->push_back(o->clone());
    }
    return *this;
}

OrdersList::~OrdersList() {
    for (Order *o : *orders) {
        delete o;
    }
    delete orders;
}

void OrdersList::add(Order *o) {
    // add takes ownership of the pointer
    orders->push_back(o);
}

bool OrdersList::remove(int index) {
    if (index < 0 || index >= static_cast<int>(orders->size())) {
        return false;
    }
    delete (*orders)[index];
    orders->erase(orders->begin() + index);
    return true;
}

bool OrdersList::move(int from, int to) {
    int size = static_cast<int>(orders->size());
    if (from < 0 || from >= size) {
        return false;
    }
    if (to < 0 || to > size) {
        return false;
    }

    Order *item = (*orders)[from];

    // erase original position
    orders->erase(orders->begin() + from);

    // adjust insertion position when moving forward
    int insertPos = to;
    if (from < to) {
        insertPos = to - 1;
    }
    orders->insert(orders->begin() + insertPos, item);
    return true;
}

vector<Order *> *OrdersList::getOrders() const {
    return orders;
}

ostream &operator<<(ostream &os, const OrdersList &ol) {
    os << "OrdersList(size=" << ol.orders->size() << "):\n";
    for (size_t i = 0; i < ol.orders->size(); ++i) {
        Order *o = (*ol.orders)[i];
        if (o) {
            os << "  " << i << ": " << *o << "\n";
        } else {
            os << "  " << i << ": (null)\n";
        }
    }
    return os;
}


