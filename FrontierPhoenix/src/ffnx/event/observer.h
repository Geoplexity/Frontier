#ifndef FRONTIER_PHOENIX_EVENT_OBSERVER_H
#define FRONTIER_PHOENIX_EVENT_OBSERVER_H

namespace ffnx::event {

    using ObserverToken = long;

    template <typename ...TObserverArgs>
    class Subject {
    private:
        using TObserver = typename std::function<void(TObserverArgs...)>;

        std::map<ObserverToken, TObserver> observers;
        ObserverToken nextToken = 0;

    public:

        ObserverToken attachObserver(TObserver observer) {
            auto result = nextToken;

            observers[result] = observer;

            nextToken++;
            return result;
        }

        void detachObserver(const ObserverToken &token) {
            observers.erase(token);
        }

        void notify(TObserverArgs... args) {
            for (auto& kv : observers) {
                kv.second(args...);
            }
        }
    };
}

#endif