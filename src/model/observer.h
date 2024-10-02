#ifndef OBSERVER_H
#define OBSERVER_H

namespace s21 {
class Observer {  // слушает
 public:
  virtual void update_state() = 0;  // Метод обновления, который вызывается
                                    // субъектом для уведомления наблюдателей
};
}  // namespace s21

#endif  // OBSERVER_H
