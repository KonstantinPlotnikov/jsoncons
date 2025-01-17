### jsoncons::json_type_traits

```c++
#include <jsoncons/json_type_traits.hpp>
```

`json_type_traits` defines a compile time template based interface for accessing and modifying `basic_json` values.

The default definition provided by the `jsoncons` library is

```c++
template <class Json, class T, class Enable=void>
struct json_type_traits
{
    typedef typename Json::allocator_type allocator_type;

    static constexpr bool is_compatible = false;

    static constexpr bool is(const Json&)
    {
        return false;
    }

    static T as(const Json&);

    static Json to_json(const T&, allocator_type = allocator_type());
};
```

You can interact with a new type using `is<T>`, `as<T>`, construction and assignment by specializing `json_type_traits` in the `jsoncons` namespace.

If you try to specialize `json_type_traits` for a type that is already
specialized in the jsoncons library, for example, a custom container 
type that satisfies the conditions for a sequence container, you 
may see a compile error "more than one partial specialization matches the template argument list".
For these situations `jsoncons` provides the traits class
```c++
template <class T>
struct is_json_type_traits_declared : public false_type {};
```
which inherits from [false_type](http://www.cplusplus.com/reference/type_traits/false_type/).
This traits class may be specialized for a user-defined type with a [true_type](http://www.cplusplus.com/reference/type_traits/true_type/) value to
inform the `jsoncons` library that the type is already specialized.  

### Specializations

`T`|`j.is<T>()`|`j.as<T>()`|j is assignable from `T`
--------|-----------|--------------|---
`Json`|`true`|self|<em>&#x2713;</em>
`Json::object`|`true` if `j.is_object()`, otherwise `false`|Compile-time error|<em>&#x2713;</em>
`Json::array`|`true` if `j.is_array()`, otherwise `false`|Compile-time error|<em>&#x2713;</em>
`bool`|`true` if `j.is_bool()`, otherwise `false`|as `bool`|<em>&#x2713;</em>
`null_type`|`true` if `j.is_null()`, otherwise `false`|`null_type()` value if j.is_null(), otherwise throws|<em>&#x2713;</em>
`const char_type*`|`true` if string, otherwise `false`|as `const char_type*`|<em>&#x2713;</em>
`char_type*`|`true` if `j.is_string()`, otherwise `false`|Compile-time error|<em>&#x2713;</em>
`integral types`|`true` if `j.is_int64()` or `j.is_uint64()` and value is in range, otherwise `false`|j numeric value cast to `T`|<em>&#x2713;</em>
`floating point types`|`true` if j.is_double() and value is in range, otherwise `false`|j numeric value cast to `T`|<em>&#x2713;</em>
`string`|`true` if j.is_string(), otherwise `false`|as string|<em>&#x2713;</em>
STL sequence container (other than string) e.g. std::vector|`true` if array and each value is assignable to a `Json` value, otherwise `false`|if array and each value is convertible to `value_type`, as container, otherwise throws|<em>&#x2713;</em>
STL associative container e.g. std::map|`true` if object and each `mapped_type` is assignable to `Json`, otherwise `false`|if object and each member value is convertible to `mapped_type`, as container|<em>&#x2713;</em>
`std::tuple`|`true` if `j.is_array()` and each array element is assignable to the corresponding `tuple` element, otherwise false|tuple with array elements converted to tuple elements|<em>&#x2713;</em>
`std::pair`|`true` if `j.is_array()` and `j.size()==2` and each array element is assignable to the corresponding pair element, otherwise false|pair with array elements converted to pair elements|<em>&#x2713;</em>

### Convenience Macros

The `jsoncons` library provides a number of macros that can be used to generate the code to specialize `json_type_traits`
for a user-defined class.

```c++
JSONCONS_MEMBER_TRAITS_DECL(class_name,
                            member_name0,
                            member_name1,...) // (1)

JSONCONS_STRICT_MEMBER_TRAITS_DECL(class_name,
                                   member_name0,
                                   member_name1,...) // (2)

JSONCONS_MEMBER_NAMED_TRAITS_DECL(class_name,
                                  (member_name0,"name0"),
                                  (member_name1,"name1")...) // (3)

JSONCONS_STRICT_MEMBER_NAMED_TRAITS_DECL(class_name,
                                         (member_name0,"name0"),
                                         (member_name1,"name1")...) // (4)

JSONCONS_TPL_MEMBER_TRAITS_DECL(num_template_params,
                                class_name,
                                member_name0,
                                member_name1,...) // (5)  

JSONCONS_STRICT_TPL_MEMBER_TRAITS_DECL(num_template_params,
                                       class_name,
                                       member_name0,
                                       member_name1,...) // (6)

JSONCONS_TPL_MEMBER_NAMED_TRAITS_DECL(num_template_params,
                                      class_name,
                                      (member_name0,"name0"),
                                      (member_name1,"name1")...) // (7)

JSONCONS_STRICT_TPL_MEMBER_NAMED_TRAITS_DECL(num_template_params,
                                             class_name,
                                             (member_name0,"name0"),
                                             (member_name1,"name1")...) // (8)

JSONCONS_ENUM_TRAITS_DECL(enum_name,
                          identifier0,
                          identifier1,...) // (9)

JSONCONS_ENUM_NAMED_TRAITS_DECL(enum_name,
                                (identifier0,"name0"),
                                (identifier1,"name1")...) // (10)

JSONCONS_GETTER_CTOR_TRAITS_DECL(class_name,
                                 getter_name0,
                                 getter_name1,...) // (11)

JSONCONS_GETTER_CTOR_NAMED_TRAITS_DECL(class_name,
                                      (getter_name0,"name0"),
                                      (getter_name1,"name1")...) // (12)

JSONCONS_TPL_GETTER_CTOR_TRAITS_DECL(num_template_params,
                                     class_name,getter_name0,
                                     getter_name1,...) // (13)

JSONCONS_TPL_GETTER_CTOR_NAMED_TRAITS_DECL(num_template_params,
                                           class_name,
                                           (getter_name0,"name0"),
                                           (getter_name1,"name1")...) // (14)

JSONCONS_GETTER_SETTER_NAMED_TRAITS_DECL(class_name,
                                         (getter_name0,setter_name0,"name0"),
                                         (getter_name1,setter_name1,"name1")...) // (15)

JSONCONS_STRICT_GETTER_SETTER_NAMED_TRAITS_DECL(class_name,
                                                (getter_name0,setter_name0,"name0"),
                                                (getter_name1,setter_name1,"name1")...) // (16)

JSONCONS_TPL_GETTER_SETTER_NAMED_TRAITS_DECL(num_template_params,
                                             class_name,
                                             (getter_name0,setter_name0,"name0"),
                                             (getter_name1,setter_name1,"name1")...) // (17)

JSONCONS_STRICT_TPL_GETTER_SETTER_NAMED_TRAITS_DECL(num_template_params,
                                                    class_name,
                                                    (getter_name0,setter_name0,"name0"),
                                                    (getter_name1,setter_name1,"name1")...) // (18)
```

(1) - (4) generate the code to specialize `json_type_traits` from the member data of a class. 
(1) and (2) will serialize to the stringified member names, (3) and (4) will serialize to the given names. 
When decoding to a C++ data structure, 
(1) and (3) allow member names not present in the JSON to have default values, while the strict (2) and (4) 
require that all member names be present in the JSON. The class must have a default constructor.
If the member data or default constructor are private, the macro `JSONCONS_TYPE_TRAITS_FRIEND`
will make them accessible to `json_type_traits`, used so
 
```c++
class MyClass
{
    JSONCONS_TYPE_TRAITS_FRIEND;
...
};
```

(5) - (8) generate the code to specialize `json_type_traits` from the member data of a class template. 
(5) and (6) will serialize to the stringified member names, (7) and (8) will serialize to the given names. 
The `num_template_params` parameter specifies the number of template parameters.
When decoding to a C++ data structure, 
(5) and (7) allow member names not present in the JSON to have default values, while the strict (6) and (8) 
require that all member names be present in the JSON. The class template must have a default constructor.
If the member data or default constructor are private, the macro `JSONCONS_TYPE_TRAITS_FRIEND`
will make them accessible to `json_type_traits`.

(9) - (10) generate the code to specialize `json_type_traits` from the identifiers of an enumeration.
(9) will serialize to the stringified identifier names, (10) will serialize to the given names. 

(11) - (12) generate the code to specialize `json_type_traits` from the getter functions and a constructor of a class. 
(11) will serialize to the stringified getter names, (12) will serialize to the given names. 
When decoding to a C++ data strucure, all data members in the C++ object must be present in the JSON.
The class must have a constructor such that the return types of the getter functions are convertible to
its parameters, taken in order. 

(13) - (14) generate the code to specialize `json_type_traits` from the getter functions and a constructor of a
class template. (13) will serialize to the stringified getter names, (14) will serialize to the given names. 
The `num_template_params` parameter specifies the number of template parameters.
When decoding to a C++ data strucure, all data members in the C++ object must be present in the JSON.
The class template must have a constructor such that the return types of the getter functions are convertible to
its parameters, taken in order. 

(15) - (16) generate the code to specialize `json_type_traits` from the getter and setter functions of a
class, and will serialize to the given names. When decoding to a C++ data structure, 
(15) allows member names not present in the JSON to have default values, while the strict (16) 
requires that all member names be present in the JSON. The class must have a default constructor. 

(17) - (18) generate the code to specialize `json_type_traits` from the getter and setter functions of a
class template, and will serialize to the given names. 
The `num_template_params` parameter specifies the number of template parameters.
When decoding to a C++ data structure, 
(17) allows member names not present in the JSON to have default values, while the strict (18) 
requires that all member names be present in the JSON. The class template must have a default constructor. 

#### Parameters

`class_name` - the name of a class or struct  
`enum_name` - the name of an enum type or enum class type
`num_template_params` - for a class template, the number of template parameters  
`member_nameN` - the name of a class data member  
`getter_nameN` - the getter for a class data member 
`(identifierN,"nameN")` - an enum identifier and corresponding JSON name  
`(getter_nameN,"nameN")` - the getter for a class data member and corresponding JSON name  
`(getter_nameN,setter_nameN,"nameN")` - the getter and setter for a class data member, and corresponding JSON name

These macro declarations must be placed at global scope, outside any namespace blocks, and `class_name` must be a fully namespace qualified name.

### Examples

[Convert from and to standard library sequence containers](#A1)  
[Convert from and to standard library associative containers](#A2)  
[Convert from and to std::tuple](#A3)  
[Extend json_type_traits to support `boost::gregorian` dates.](#A4)  
[Specialize json_type_traits to support a book class.](#A5)  
[Using JSONCONS_MEMBER_TRAITS_DECL to generate the json_type_traits](#A6)  
[A polymorphic example using JSONCONS_GETTER_CTOR_TRAITS_DECL to generate the json_type_traits](#A7)  
[Specialize json_type_traits for a container type that the jsoncons library also supports](#A8)  
[Convert JSON to/from boost matrix](#A9)

<div id="A1"/> 

#### Convert from and to standard library sequence containers

```c++
    std::vector<int> v{1, 2, 3, 4};
    json j(v);
    std::cout << "(1) "<< j << std::endl;
    std::deque<int> d = j.as<std::deque<int>>();
```
Output:
```
(1) [1,2,3,4]
```

<div id="A2"/> 

#### Convert from and to standard library associative containers

```c++
    std::map<std::string,int> m{{"one",1},{"two",2},{"three",3}};
    json j(m);
    std::cout << j << std::endl;
    std::unordered_map<std::string,int> um = j.as<std::unordered_map<std::string,int>>();
```
Output:
```
{"one":1,"three":3,"two":2}
```

<div id="A3"/> 

#### Convert from and to std::tuple

```c++
    auto t = std::make_tuple(false,1,"foo");
    json j(t);
    std::cout << j << std::endl;
    auto t2 = j.as<std::tuple<bool,int,std::string>>();
```
Output:
```
[false,1,"foo"]
```

<div id="A4"/> 

#### Extend json_type_traits to support `boost::gregorian` dates.

```c++
#include <jsoncons/json.hpp>
#include "boost/datetime/gregorian/gregorian.hpp"

namespace jsoncons 
{
    template <class Json>
    struct json_type_traits<Json,boost::gregorian::date>
    {
        static const bool is_assignable = true;

        static bool is(const Json& val) noexcept
        {
            if (!val.is_string())
            {
                return false;
            }
            std::string s = val.template as<std::string>();
            try
            {
                boost::gregorian::from_simple_string(s);
                return true;
            }
            catch (...)
            {
                return false;
            }
        }

        static boost::gregorian::date as(const Json& val)
        {
            std::string s = val.template as<std::string>();
            return boost::gregorian::from_simple_string(s);
        }

        static Json to_json(boost::gregorian::date val)
        {
            return Json(to_iso_extended_string(val));
        }
    };
}
```
```c++
namespace ns
{
    using jsoncons::json;
    using boost::gregorian::date;

    json deal = json::parse(R"(
    {
        "Maturity":"2014-10-14",
        "ObservationDates": ["2014-02-14","2014-02-21"]
    }
    )");

    deal["ObservationDates"].push_back(date(2014,2,28));    

    date maturity = deal["Maturity"].as<date>();
    std::cout << "Maturity: " << maturity << std::endl << std::endl;

    std::cout << "Observation dates: " << std::endl << std::endl;

    for (auto observation_date: deal["ObservationDates"].array_range())
    {
        std::cout << observation_date << std::endl;
    }
    std::cout << std::endl;
}
```
Output:
```
Maturity: 2014-Oct-14

Observation dates:

2014-Feb-14
2014-Feb-21
2014-Feb-28
```

<div id="A5"/> 

#### Specialize json_type_traits to support a book class.

```c++
#include <iostream>
#include <jsoncons/json.hpp>
#include <vector>
#include <string>

namespace ns {
    struct book
    {
        std::string author;
        std::string title;
        double price;
    };
} // namespace ns

namespace jsoncons {

    template<class Json>
    struct json_type_traits<Json, ns::book>
    {
        typedef typename Json::allocator_type allocator_type;

        static bool is(const Json& j) noexcept
        {
            return j.is_object() && j.contains("author") && 
                   j.contains("title") && j.contains("price");
        }
        static ns::book as(const Json& j)
        {
            ns::book val;
            val.author = j.at("author").template as<std::string>();
            val.title = j.at("title").template as<std::string>();
            val.price = j.at("price").template as<double>();
            return val;
        }
        static Json to_json(const ns::book& val, 
                            allocator_type allocator=allocator_type())
        {
            Json j(allocator);
            j.try_emplace("author", val.author);
            j.try_emplace("title", val.title);
            j.try_emplace("price", val.price);
            return j;
        }
    };
} // namespace jsoncons
```

To save typing and enhance readability, the jsoncons library defines macros, 
so you could also write

```c++
JSONCONS_MEMBER_TRAITS_DECL(ns::book, author, title, price)
```

which expands to the code above.

```c++
using namespace jsoncons; // for convenience

int main()
{
    const std::string s = R"(
    [
        {
            "author" : "Haruki Murakami",
            "title" : "Kafka on the Shore",
            "price" : 25.17
        },
        {
            "author" : "Charles Bukowski",
            "title" : "Pulp",
            "price" : 22.48
        }
    ]
    )";

    std::vector<ns::book> book_list = decode_json<std::vector<ns::book>>(s);

    std::cout << "(1)\n";
    for (const auto& item : book_list)
    {
        std::cout << item.author << ", " 
                  << item.title << ", " 
                  << item.price << "\n";
    }

    std::cout << "\n(2)\n";
    encode_json(book_list, std::cout, indenting::indent);
    std::cout << "\n\n";
}
```
Output:
```
(1)
Haruki Murakami, Kafka on the Shore, 25.17
Charles Bukowski, Pulp, 22.48

(2)
[
    {
        "author": "Haruki Murakami",
        "price": 25.17,
        "title": "Kafka on the Shore"
    },
    {
        "author": "Charles Bukowski",
        "price": 22.48,
        "title": "Pulp"
    }
]
```

<div id="A6"/> 

#### Using JSONCONS_MEMBER_TRAITS_DECL to generate the json_type_traits 

`JSONCONS_MEMBER_TRAITS_DECL` is a macro that can be used to generate the `json_type_traits` boilerplate
for your own types.

```c++
#include <cassert>
#include <iostream>
#include <jsoncons/json.hpp>

namespace ns {
    enum class hiking_experience {beginner,intermediate,advanced};

    class hiking_reputon
    {
        std::string rater_;
        hiking_experience assertion_;
        std::string rated_;
        double rating_;
    public:
        hiking_reputon(const std::string& rater,
                       hiking_experience assertion,
                       const std::string& rated,
                       double rating)
            : rater_(rater), assertion_(assertion), rated_(rated), rating_(rating)
        {
        }

        const std::string& rater() const {return rater_;}
        hiking_experience assertion() const {return assertion_;}
        const std::string& rated() const {return rated_;}
        double rating() const {return rating_;}

        friend bool operator==(const hiking_reputon& lhs, const hiking_reputon& rhs)
        {
            return lhs.rater_ == rhs.rater_ && lhs.assertion_ == rhs.assertion_ && 
                   lhs.rated_ == rhs.rated_ && lhs.rating_ == rhs.rating_;
        }

        friend bool operator!=(const hiking_reputon& lhs, const hiking_reputon& rhs)
        {
            return !(lhs == rhs);
        };
    };

    class hiking_reputation
    {
        std::string application_;
        std::vector<hiking_reputon> reputons_;
    public:
        hiking_reputation(const std::string& application, 
                          const std::vector<hiking_reputon>& reputons)
            : application_(application), 
              reputons_(reputons)
        {}

        const std::string& application() const { return application_;}
        const std::vector<hiking_reputon>& reputons() const { return reputons_;}

        friend bool operator==(const hiking_reputation& lhs, const hiking_reputation& rhs)
        {
            return (lhs.application_ == rhs.application_) && (lhs.reputons_ == rhs.reputons_);
        }

        friend bool operator!=(const hiking_reputation& lhs, const hiking_reputation& rhs)
        {
            return !(lhs == rhs);
        };
    };

} // namespace ns

using namespace jsoncons; // for convenience

// Declare the traits. Specify which data members need to be serialized.
JSONCONS_ENUM_TRAITS_DECL(ns::hiking_experience, beginner, intermediate, advanced)
JSONCONS_GETTER_CTOR_TRAITS_DECL(ns::hiking_reputon, rater, assertion, rated, rating)
JSONCONS_GETTER_CTOR_TRAITS_DECL(ns::hiking_reputation, application, reputons)

int main()
{
    ns::hiking_reputation val("hiking", { ns::hiking_reputon{"HikingAsylum",ns::hiking_experience::advanced,"Marilyn C",0.90} });

    std::string s;
    encode_json(val, s, indenting::indent);
    std::cout << s << "\n";

    auto val2 = decode_json<ns::hiking_reputation>(s);

    assert(val2 == val);
}
```
Output:
```
{
    "application": "hiking",
    "reputons": [
        {
            "assertion": "advanced",
            "rated": "Marilyn C",
            "rater": "HikingAsylum",
            "rating": 0.9
        }
    ]
}
```

<div id="A7"/> 

#### A polymorphic example using JSONCONS_GETTER_CTOR_TRAITS_DECL to generate the json_type_traits

`JSONCONS_GETTER_CTOR_TRAITS_DECL` is a macro that can be used to generate the `json_type_traits` boilerplate
from the getter functions and a constructor.

```c++
#include <cassert>
#include <iostream>
#include <vector>
#include <jsoncons/json.hpp>

using namespace jsoncons;

namespace ns {

class Employee
{
    std::string firstName_;
    std::string lastName_;
public:
    Employee(const std::string& firstName, const std::string& lastName)
        : firstName_(firstName), lastName_(lastName)
    {
    }
    virtual ~Employee() = default;

    virtual double calculatePay() const = 0;

    const std::string& firstName() const {return firstName_;}
    const std::string& lastName() const {return lastName_;}
};

class HourlyEmployee : public Employee
{
    double wage_;
    unsigned hours_;
public:
    HourlyEmployee(const std::string& firstName, const std::string& lastName, 
                   double wage, unsigned hours)
        : Employee(firstName, lastName), 
          wage_(wage), hours_(hours)
    {
    }
    HourlyEmployee(const HourlyEmployee&) = default;
    HourlyEmployee(HourlyEmployee&&) = default;
    HourlyEmployee& operator=(const HourlyEmployee&) = default;
    HourlyEmployee& operator=(HourlyEmployee&&) = default;

    double wage() const {return wage_;}

    unsigned hours() const {return hours_;}

    double calculatePay() const override
    {
        return wage_*hours_;
    }
};

class CommissionedEmployee : public Employee
{
    double baseSalary_;
    double commission_;
    unsigned sales_;
public:
    CommissionedEmployee(const std::string& firstName, const std::string& lastName, 
                         double baseSalary, double commission, unsigned sales)
        : Employee(firstName, lastName), 
          baseSalary_(baseSalary), commission_(commission), sales_(sales)
    {
    }
    CommissionedEmployee(const CommissionedEmployee&) = default;
    CommissionedEmployee(CommissionedEmployee&&) = default;
    CommissionedEmployee& operator=(const CommissionedEmployee&) = default;
    CommissionedEmployee& operator=(CommissionedEmployee&&) = default;

    double baseSalary() const
    {
        return baseSalary_;
    }

    double commission() const
    {
        return commission_;
    }

    unsigned sales() const
    {
        return sales_;
    }

    double calculatePay() const override
    {
        return baseSalary_ + commission_*sales_;
    }
};
} // ns

JSONCONS_GETTER_CTOR_TRAITS_DECL(ns::HourlyEmployee, firstName, lastName, wage, hours)
JSONCONS_GETTER_CTOR_TRAITS_DECL(ns::CommissionedEmployee, firstName, lastName, baseSalary, commission, sales)

namespace jsoncons {

template<class Json>
struct json_type_traits<Json, std::shared_ptr<ns::Employee>> 
{
    static bool is(const Json& j) noexcept
    { 
        return j.is<ns::HourlyEmployee>() || j.is<ns::CommissionedEmployee>();
    }
    static std::shared_ptr<ns::Employee> as(const Json& j)
    {   
        if (j.at("type").as<std::string>() == "Hourly")
        {
            return std::make_shared<ns::HourlyEmployee>(j.as<ns::HourlyEmployee>());
        }
        else if (j.at("type").as<std::string>() == "Commissioned")
        {
            return std::make_shared<ns::CommissionedEmployee>(j.as<ns::CommissionedEmployee>());
        }
        else
        {
            throw std::runtime_error("Not an employee");
        }
    }
    static Json to_json(const std::shared_ptr<ns::Employee>& ptr)
    {
        if (ns::HourlyEmployee* p = dynamic_cast<ns::HourlyEmployee*>(ptr.get()))
        {
            Json j(*p);
            j.try_emplace("type","Hourly");
            return j;
        }
        else if (ns::CommissionedEmployee* p = dynamic_cast<ns::CommissionedEmployee*>(ptr.get()))
        {
            Json j(*p);
            j.try_emplace("type","Commissioned");
            return j;
        }
        else
        {
            throw std::runtime_error("Not an employee");
        }
	}
};

} // jsoncons

int main()
{
    std::vector<std::shared_ptr<ns::Employee>> v;

    v.push_back(std::make_shared<ns::HourlyEmployee>("John", "Smith", 40.0, 1000));
    v.push_back(std::make_shared<ns::CommissionedEmployee>("Jane", "Doe", 30000, 0.25, 1000));

    json j(v);
    std::cout << pretty_print(j) << "\n\n";

    assert(j[0].is<ns::HourlyEmployee>());
    assert(!j[0].is<ns::CommissionedEmployee>());
    assert(!j[1].is<ns::HourlyEmployee>());
    assert(j[1].is<ns::CommissionedEmployee>());


    for (size_t i = 0; i < j.size(); ++i)
    {
        auto p = j[i].as<std::shared_ptr<ns::Employee>>();
        assert(p->firstName() == v[i]->firstName());
        assert(p->lastName() == v[i]->lastName());
        assert(p->calculatePay() == v[i]->calculatePay());
    }
}
```
Output:
```
[
    {
        "firstName": "John",
        "hours": 1000,
        "lastName": "Smith",
        "type": "Hourly",
        "wage": 40.0
    },
    {
        "baseSalary": 30000.0,
        "commission": 0.25,
        "firstName": "Jane",
        "lastName": "Doe",
        "sales": 1000,
        "type": "Commissioned"
    }
]
```

<div id="A8"/> 

#### Specialize json_type_traits for a container type that the jsoncons library also supports

```c++
#include <cassert>
#include <string>
#include <vector>
#include <jsoncons/json.hpp>

//own vector will always be of an even length 
struct own_vector : std::vector<int64_t> { using  std::vector<int64_t>::vector; };

namespace jsoncons {

template<class Json>
struct json_type_traits<Json, own_vector> 
{
    static bool is(const Json& j) noexcept
    { 
        return j.is_object() && j.size() % 2 == 0;
    }
    static own_vector as(const Json& j)
    {   
        own_vector v;
        for (auto& item : j.object_range())
        {
            std::string s(item.key());
            v.push_back(std::strtol(s.c_str(),nullptr,10));
            v.push_back(item.value().template as<int64_t>());
        }
        return v;
    }
    static Json to_json(const own_vector& val){
		Json j;
		for(size_t i=0;i<val.size();i+=2){
			j[std::to_string(val[i])] = val[i + 1];
		}
		return j;
	}
};

template <> 
struct is_json_type_traits_declared<own_vector> : public std::true_type 
{}; 
} // jsoncons

using jsoncons::json;

int main()
{
    json j = json::object{ {"1",2},{"3",4} };
    assert(j.is<own_vector>());
    auto v = j.as<own_vector>();
    json j2 = v;

    std::cout << j2 << "\n";
}
```
Output:
```
{"1":2,"3":4}
```

<div id="A9"/>

#### Convert JSON to/from boost matrix

```c++
#include <jsoncons/json.hpp>
#include <boost/numeric/ublas/matrix.hpp>

namespace jsoncons {

    template <class Json, class T>
    struct json_type_traits<Json,boost::numeric::ublas::matrix<T>>
    {
        static bool is(const Json& val) noexcept
        {
            if (!val.is_array())
            {
                return false;
            }
            if (val.size() > 0)
            {
                size_t n = val[0].size();
                for (const auto& a: val.array_range())
                {
                    if (!(a.is_array() && a.size() == n))
                    {
                        return false;
                    }
                    for (auto x: a.array_range())
                    {
                        if (!x.template is<T>())
                        {
                            return false;
                        }
                    }
                }
            }
            return true;
        }

        static boost::numeric::ublas::matrix<T> as(const Json& val)
        {
            if (val.is_array() && val.size() > 0)
            {
                size_t m = val.size();
                size_t n = 0;
                for (const auto& a : val.array_range())
                {
                    if (a.size() > n)
                    {
                        n = a.size();
                    }
                }

                boost::numeric::ublas::matrix<T> A(m,n,T());
                for (size_t i = 0; i < m; ++i)
                {
                    const auto& a = val[i];
                    for (size_t j = 0; j < a.size(); ++j)
                    {
                        A(i,j) = a[j].template as<T>();
                    }
                }
                return A;
            }
            else
            {
                boost::numeric::ublas::matrix<T> A;
                return A;
            }
        }

        static Json to_json(const boost::numeric::ublas::matrix<T>& val)
        {
            Json a = Json::template make_array<2>(val.size1(), val.size2(), T());
            for (size_t i = 0; i < val.size1(); ++i)
            {
                for (size_t j = 0; j < val.size1(); ++j)
                {
                    a[i][j] = val(i,j);
                }
            }
            return a;
        }
    };
} // jsoncons

using namespace jsoncons;
using boost::numeric::ublas::matrix;

int main()
{
    matrix<double> A(2, 2);
    A(0, 0) = 1.1;
    A(0, 1) = 2.1;
    A(1, 0) = 3.1;
    A(1, 1) = 4.1;

    json a = A;

    std::cout << "(1) " << std::boolalpha << a.is<matrix<double>>() << "\n\n";

    std::cout << "(2) " << std::boolalpha << a.is<matrix<int>>() << "\n\n";

    std::cout << "(3) \n\n" << pretty_print(a) << "\n\n";

    matrix<double> B = a.as<matrix<double>>();

    std::cout << "(4) \n\n";
    for (size_t i = 0; i < B.size1(); ++i)
    {
        for (size_t j = 0; j < B.size2(); ++j)
        {
            if (j > 0)
            {
                std::cout << ",";
            }
            std::cout << B(i, j);
        }
        std::cout << "\n";
    }
    std::cout << "\n\n";
}
```
Output:
```
(1) true

(2) false

(3)

[
    [1.1,2.1],
    [3.1,4.1]
]

(4)

1.1,2.1
3.1,4.1
``` 

