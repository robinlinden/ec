#pragma once

#include "ec/core/active_object.h"

#include <any>
#include <functional>
#include <typeindex>
#include <unordered_map>

namespace ec::core {

struct Subscription {
    void *handle{nullptr};
};

class EventService {
public:
    explicit EventService(ActiveObject *ctx) : ctx_{ctx} {}

    template<typename EventT, typename HandlerT>
    Subscription subscribe(HandlerT &&h) {
        const auto idx = std::type_index(typeid(EventT));
        auto it = handlers.emplace(idx, [this, func = std::forward<HandlerT>(h)](auto val) {
            ctx_->call([=] { func(std::any_cast<EventT>(val)); });
        });

        return {&it->second};
    }


    template<typename EventT, typename ClassT, typename Method>
    Subscription subscribe(ClassT *instance, Method &&method) {
        const auto idx = std::type_index(typeid(EventT));
        auto it = handlers.emplace(idx, [=, this](auto value) {
            ctx_->call([=] { (instance->*method)(std::any_cast<EventT>(value)); });
        });

        return {&it->second};
    }

    template<typename EventT>
    void dispatch(EventT event) {
        const auto idx = std::type_index(typeid(EventT));
        for (auto[id, last] = handlers.equal_range(idx); id != last; ++id) {
            id->second(event);
        }
    }

    void unsubscribe(const Subscription &s) {
        for (auto it = handlers.begin(); it != handlers.end(); ++it) {
            if (&it->second == s.handle) {
                handlers.erase(it);
                return;
            }
        }
    }

private:
    ActiveObject *const ctx_;

    std::unordered_multimap<
            std::type_index,
            std::function<void(std::any)>> handlers{};
};

} // namespace ec::core
