#ifndef OBSERVABLE_H
#define OBSERVABLE_H

#include "observer.h"

namespace s21 {
class Observable {  // кого слушают
 public:
  virtual void register_observer(Observer* observer) = 0;
  virtual void remove_observer(Observer* observer) = 0;
  virtual void notify_observers() = 0;
};
}  // namespace s21

#endif  // OBSERVABLE_H
