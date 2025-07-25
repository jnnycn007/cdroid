/*********************************************************************************
 * Copyright (C) [2019] [houzh@msn.com]
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *********************************************************************************/
#ifndef __CDROID_PREDICATE_H__
#define __CDROID_PREDICATE_H__
#include <functional>
namespace cdroid{

template<typename T>
class Predicate {
private:
    std::function<bool(const T&)> func_;
public:
    Predicate(std::function<bool(const T&)> func) : func_(std::move(func)) {}

    Predicate() = default;

    virtual bool test(const T& t)const{
        if (func_) {
            return func_(t);
        }
        return true;
    }

    Predicate<T> operator&&(const Predicate<T>& other) const {
        return Predicate<T>([this, &other](const T& t) {
            return this->test(t) && other.test(t);
        });
    }

    Predicate<T> operator||(const Predicate<T>& other) const {
        return Predicate<T>([this, &other](const T& t) {
            return this->test(t) || other.test(t);
        });
    }

    Predicate<T> operator!() const {
        return Predicate<T>([this](const T& t) {
            return!this->test(t);
        });
    }
    static Predicate<T> isEqual(const T& value) {
        return Predicate<T>([value](const T& t) {
            return t == value;
        });
    }
};

/*int main(int argc,char*argv[]) {
    Predicate<int> p1 = Predicate<int>::isEqual(5);
    Predicate<int> p2 = Predicate<int>::isEqual(10);

    auto p3 = p1 && p2; // p3 是 p1 和 p2 的逻辑与
    auto p4 = p1 || p2; // p4 是 p1 和 p2 的逻辑或
    auto p5 = !p1;      // p5 是 p1 的逻辑非

    std::cout << "p3(5): " << p3.test(5) << std::endl; // 输出 0 (false)
    std::cout << "p4(5): " << p4.test(5) << std::endl; // 输出 1 (true)
    std::cout << "p5(5): " << p5.test(5) << std::endl; // 输出 0 (false)

    return 0;
}*/

}/*endof namespace*/
#endif/*__CDROID_PREDICATE_H__*/
