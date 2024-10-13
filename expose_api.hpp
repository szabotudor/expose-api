#pragma once
#include <cassert>
#include <cstdint>
#include <functional>
#include <queue>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <any>


/// ==============================
/// Thanks to https://github.com/MitalAshok/self_macro/ (by user MitalAshok on GitHub) for being the first to discover the solution to the self macro.
/// Original implementation by MitalAshok is licensed under MIT License. Original license can be found on the repository linked above.
#if !defined(DEFINE_SELF)
namespace SelfType
    {
        void __get_self_type(...);

        template <typename T>
        struct Reader
        {
            friend auto __get_self_type(Reader<T>);
        };

        template <typename T, typename U>
        struct Writer
        {
            friend auto __get_self_type(Reader<T>){return U{};}
        };

        template <typename T>
        using Read = std::remove_pointer_t<decltype(__get_self_type(Reader<T>{}))>;
    }

#define DEFINE_SELF \
    constexpr auto __self_type_helper() -> decltype(SelfType::Writer<struct __self_type_tag, decltype(this)>{}); \
    using Self = SelfType::Read<__self_type_tag>;

#define DEFINE_SELF_WITH_NAME(name) \
    constexpr auto __##name##_type_helper() -> decltype(SelfType::Writer<struct __self_type_tag, decltype(this)>{}) { return {}; } \
    using name = SelfType::Read<__self_type_tag>;
#endif
/// ==============================


#define APPLY(macro, arg) macro(arg)

//==================================================================================================
// Thanks to Luiz Martins for the VAR_COUNT macro, in this answer on stackoverflow: https://stackoverflow.com/a/66556553
/* NOTE: In these macros, "1" means true, and "0" means false. */

#define EXPAND(x) x

#define _GLUE(X,Y) X##Y
#define GLUE(X,Y) _GLUE(X,Y)

/* Returns the 100th argument. */
#define _ARG_100(_,\
   _100,_99,_98,_97,_96,_95,_94,_93,_92,_91,_90,_89,_88,_87,_86,_85,_84,_83,_82,_81, \
   _80,_79,_78,_77,_76,_75,_74,_73,_72,_71,_70,_69,_68,_67,_66,_65,_64,_63,_62,_61, \
   _60,_59,_58,_57,_56,_55,_54,_53,_52,_51,_50,_49,_48,_47,_46,_45,_44,_43,_42,_41, \
   _40,_39,_38,_37,_36,_35,_34,_33,_32,_31,_30,_29,_28,_27,_26,_25,_24,_23,_22,_21, \
   _20,_19,_18,_17,_16,_15,_14,_13,_12,_11,_10,_9,_8,_7,_6,_5,_4,_3,_2,X_,...) X_

/* Returns whether __VA_ARGS__ has a comma (up to 100 arguments). */
#define HAS_COMMA(...) EXPAND(_ARG_100(__VA_ARGS__, \
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,1, \
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0))

/* Produces a comma if followed by a parenthesis. */
#define _TRIGGER_PARENTHESIS_(...) ,
#define _PASTE5(_0, _1, _2, _3, _4) _0 ## _1 ## _2 ## _3 ## _4
#define _IS_EMPTY_CASE_0001 ,
/* Returns true if inputs expand to (false, false, false, true) */
#define _IS_EMPTY(_0, _1, _2, _3) HAS_COMMA(_PASTE5(_IS_EMPTY_CASE_, _0, _1, _2, _3))
/* Returns whether __VA_ARGS__ is empty. */
#define IS_EMPTY(...)                                               \
   _IS_EMPTY(                                                       \
      /* Testing for an argument with a comma                       \
         e.g. "ARG1, ARG2", "ARG1, ...", or "," */                  \
      HAS_COMMA(__VA_ARGS__),                                       \
      /* Testing for an argument around parenthesis                 \
         e.g. "(ARG1)", "(...)", or "()" */                         \
      HAS_COMMA(_TRIGGER_PARENTHESIS_ __VA_ARGS__),                 \
      /* Testing for a macro as an argument, which will             \
         expand the parenthesis, possibly generating a comma. */    \
      HAS_COMMA(__VA_ARGS__ (/*empty*/)),                           \
      /* If all previous checks are false, __VA_ARGS__ does not     \
         generate a comma by itself, nor with _TRIGGER_PARENTHESIS_ \
         behind it, nor with () after it.                           \
         Therefore, "_TRIGGER_PARENTHESIS_ __VA_ARGS__ ()"          \
         only generates a comma if __VA_ARGS__ is empty.            \
         So, this tests for an empty __VA_ARGS__ (given the         \
         previous conditionals are false). */                       \
      HAS_COMMA(_TRIGGER_PARENTHESIS_ __VA_ARGS__ (/*empty*/))      \
   )

#define _VAR_COUNT_EMPTY_1(...) 0
#define _VAR_COUNT_EMPTY_0(...) EXPAND(_ARG_100(__VA_ARGS__, \
   100,99,98,97,96,95,94,93,92,91,90,89,88,87,86,85,84,83,82,81, \
   80,79,78,77,76,75,74,73,72,71,70,69,68,67,66,65,64,63,62,61, \
   60,59,58,57,56,55,54,53,52,51,50,49,48,47,46,45,44,43,42,41, \
   40,39,38,37,36,35,34,33,32,31,30,29,28,27,26,25,24,23,22,21, \
   20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1))
#define VAR_COUNT(...) GLUE(_VAR_COUNT_EMPTY_, IS_EMPTY(__VA_ARGS__))(__VA_ARGS__)
//==================================================================================================


#define FOR_EACH_0(macro, arg)
#define FOR_EACH_1(macro, arg) APPLY(macro, arg)
#define FOR_EACH_2(macro, arg, ...) APPLY(macro, arg), FOR_EACH_1(macro, __VA_ARGS__)
#define FOR_EACH_3(macro, arg, ...) APPLY(macro, arg), FOR_EACH_2(macro, __VA_ARGS__)
#define FOR_EACH_4(macro, arg, ...) APPLY(macro, arg), FOR_EACH_3(macro, __VA_ARGS__)
#define FOR_EACH_5(macro, arg, ...) APPLY(macro, arg), FOR_EACH_4(macro, __VA_ARGS__)
#define FOR_EACH_6(macro, arg, ...) APPLY(macro, arg), FOR_EACH_5(macro, __VA_ARGS__)
#define FOR_EACH_7(macro, arg, ...) APPLY(macro, arg), FOR_EACH_6(macro, __VA_ARGS__)
#define FOR_EACH_8(macro, arg, ...) APPLY(macro, arg), FOR_EACH_7(macro, __VA_ARGS__)
#define FOR_EACH_9(macro, arg, ...) APPLY(macro, arg), FOR_EACH_8(macro, __VA_ARGS__)
#define FOR_EACH_10(macro, arg, ...) APPLY(macro, arg), FOR_EACH_9(macro, __VA_ARGS__)
#define FOR_EACH_11(macro, arg, ...) APPLY(macro, arg), FOR_EACH_10(macro, __VA_ARGS__)
#define FOR_EACH_12(macro, arg, ...) APPLY(macro, arg), FOR_EACH_11(macro, __VA_ARGS__)
#define FOR_EACH_13(macro, arg, ...) APPLY(macro, arg), FOR_EACH_12(macro, __VA_ARGS__)
#define FOR_EACH_14(macro, arg, ...) APPLY(macro, arg), FOR_EACH_13(macro, __VA_ARGS__)
#define FOR_EACH_15(macro, arg, ...) APPLY(macro, arg), FOR_EACH_14(macro, __VA_ARGS__)
#define FOR_EACH_16(macro, arg, ...) APPLY(macro, arg), FOR_EACH_15(macro, __VA_ARGS__)
#define FOR_EACH_17(macro, arg, ...) APPLY(macro, arg), FOR_EACH_16(macro, __VA_ARGS__)
#define FOR_EACH_18(macro, arg, ...) APPLY(macro, arg), FOR_EACH_17(macro, __VA_ARGS__)
#define FOR_EACH_19(macro, arg, ...) APPLY(macro, arg), FOR_EACH_18(macro, __VA_ARGS__)
#define FOR_EACH_20(macro, arg, ...) APPLY(macro, arg), FOR_EACH_19(macro, __VA_ARGS__)
#define FOR_EACH_21(macro, arg, ...) APPLY(macro, arg), FOR_EACH_20(macro, __VA_ARGS__)
#define FOR_EACH_22(macro, arg, ...) APPLY(macro, arg), FOR_EACH_21(macro, __VA_ARGS__)
#define FOR_EACH_23(macro, arg, ...) APPLY(macro, arg), FOR_EACH_22(macro, __VA_ARGS__)
#define FOR_EACH_24(macro, arg, ...) APPLY(macro, arg), FOR_EACH_23(macro, __VA_ARGS__)
#define FOR_EACH_25(macro, arg, ...) APPLY(macro, arg), FOR_EACH_24(macro, __VA_ARGS__)
#define FOR_EACH_26(macro, arg, ...) APPLY(macro, arg), FOR_EACH_25(macro, __VA_ARGS__)
#define FOR_EACH_27(macro, arg, ...) APPLY(macro, arg), FOR_EACH_26(macro, __VA_ARGS__)
#define FOR_EACH_28(macro, arg, ...) APPLY(macro, arg), FOR_EACH_27(macro, __VA_ARGS__)
#define FOR_EACH_29(macro, arg, ...) APPLY(macro, arg), FOR_EACH_28(macro, __VA_ARGS__)
#define FOR_EACH_30(macro, arg, ...) APPLY(macro, arg), FOR_EACH_29(macro, __VA_ARGS__)
#define FOR_EACH_31(macro, arg, ...) APPLY(macro, arg), FOR_EACH_30(macro, __VA_ARGS__)
#define FOR_EACH_32(macro, arg, ...) APPLY(macro, arg), FOR_EACH_31(macro, __VA_ARGS__)

#define FOR_EACH(macro, ...) \
    FOR_EACH_IMPL(macro, VAR_COUNT(__VA_ARGS__), __VA_ARGS__)

#define FOR_EACH_IMPL(macro, count, ...) FOR_EACH_IMPL_1(macro, count, __VA_ARGS__)
#define FOR_EACH_IMPL_1(macro, count, ...) FOR_EACH_##count(macro, __VA_ARGS__)


namespace mgm {
    // On MSVC, the [[no_unique_address]] attribute is replaced with [[msvc::no_unique_address]]
    #if defined(_MSC_VER)
    #define __impl_no_unique_address [[msvc::no_unique_address]]
    #else
    #define __impl_no_unique_address [[no_unique_address]]
    #endif

    class ExposeApiRuntime;

    struct ExposeApi {
        enum class ExposeTimeMode {
            CLASS, FUNCTION, VARIABLE
        };


        struct ExposedClasses {
            struct ExposedFunction {
                std::function<std::any(void*, std::vector<std::any>)> function{};
                
                struct Signature {
                    std::string traits_and_qualifiers{};
                    std::string return_type{};
                    std::string name{};
                    size_t return_type_id{};
                    
                    struct Argument {
                        std::string traits_and_qualifiers{};
                        std::string type{};
                        std::string name{};
                        size_t type_id{};
                    };
                    std::vector<Argument> arguments{};
                } signature{};

                ExposedFunction(std::function<std::any(void*, std::vector<std::any>)> init_function = {})
                    : function{init_function} {}

                std::any operator()(void* object, std::vector<std::any> args) {
                    return function(object, args);
                }
                std::any operator()(void* object, std::vector<std::any> args) const {
                    return function(object, args);
                }
            };
            struct ExposedVariable {
                uintptr_t offset{};
                std::string name{};

                ExposedVariable(uintptr_t member_offset, const std::string& member_name)
                    : offset{member_offset}, name{member_name} {}

                template<typename T>
                T& get(void* object) {
                    return *reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(object) + offset);
                }
                template<typename T>
                const T& get_const(const void* object) {
                    return *reinterpret_cast<const T*>(reinterpret_cast<uintptr_t>(object) + offset);
                }
            };
            struct ExposedMember {
                std::unordered_map<size_t, ExposedFunction>* functions{};
                ExposedVariable* variable = nullptr;
                bool is_function = false, has_value = false;
                
                template<typename... Ts>
                ExposedFunction& emplace_function(size_t type_id, Ts&&... args) {
                    if (has_value && !is_function)
                        throw std::runtime_error("Member already has a variable value");
                    if (!has_value)
                        functions = new std::unordered_map<size_t, ExposedFunction>{};
                    is_function = true;
                    has_value = true;
                    (*functions)[type_id] = ExposedFunction{std::forward<Ts>(args)...};
                    return functions->at(type_id);
                }
                template<typename... Ts>
                void emplace_variable(Ts&&... args) {
                    if (has_value)
                        return;
                    is_function = false;
                    has_value = true;
                    variable = new ExposedVariable{std::forward<Ts>(args)...};
                }

                void reset() {
                    if (!is_function)
                        delete variable;
                    is_function = false;
                    has_value = false;
                }

                ~ExposedMember() {}
            };
            struct ExposedClassMembers {
                std::unordered_map<std::string, ExposedMember> members{};
                uintptr_t runtime_offset = 0;

                struct NextFunction {
                    std::string name{};
                    size_t type_id{};

                    bool operator<(const NextFunction& other) const {
                        return name < other.name;
                    }
                    bool operator>(const NextFunction& other) const {
                        return name > other.name;
                    }
                };
                std::priority_queue<NextFunction, std::vector<NextFunction>> args_queue{};
            };
            std::unordered_map<size_t, std::string> class_names{};
            std::unordered_map<size_t, ExposedClassMembers> class_members{};
        };
        static ExposedClasses& get_exposed_classes() {
            static ExposedClasses expose_data{};
            return expose_data;
        }


        ExposeApi() = default;

        template<typename T>
        struct ExposedClassAnalyzer {
            ExposedClassAnalyzer();

            template<typename ReturnType, typename... Args>
            static bool expose(ReturnType(T::* function)(Args...), const std::string& name) {
                using TypeErasedFunctor = ReturnType(Args...);
                auto& expose_data = ExposeApi::get_exposed_classes();                
                const size_t class_id = typeid(T).hash_code();
                auto& new_function = expose_data.class_members[class_id].members[name].emplace_function(typeid(TypeErasedFunctor).hash_code(), ExposedClasses::ExposedFunction{
                    [function](void* object, std::vector<std::any> args) -> std::any{
                        assert(args.size() == sizeof...(Args) && "Invalid number of arguments for function call");
                        if constexpr (sizeof...(Args) != 0) {
                            size_t i = sizeof...(Args) - 1;
                            auto get_arg = [&args, &i]<typename Arg>() -> Arg {
                                return std::any_cast<Arg>(args[i--]);
                            };
                            if constexpr (!std::is_void_v<ReturnType>)
                                return std::any((reinterpret_cast<T*>(object)->*function)(std::forward<Args>(get_arg.template operator()<Args>())...));
                            (reinterpret_cast<T*>(object)->*function)(std::forward<Args>(get_arg.template operator()<Args>())...);
                            return {};
                        }
                        else {
                            if constexpr (!std::is_void_v<ReturnType>)
                                return std::any((reinterpret_cast<T*>(object)->*function)());
                            (reinterpret_cast<T*>(object)->*function)();
                            return {};
                        }
                    }
                });
                new_function.signature.name = name;
                new_function.signature.return_type_id = typeid(ReturnType).hash_code();
                expose_data.class_members[class_id].args_queue.push(ExposedClasses::ExposedClassMembers::NextFunction{
                    .name = name,
                    .type_id = typeid(TypeErasedFunctor).hash_code()
                });
                return true;
            }

            template<typename... Args>
            static bool expose_args(void (*)(Args...), const std::vector<std::string>& types_and_names) {
                auto& expose_data = ExposeApi::get_exposed_classes();
                const auto class_id = typeid(T).hash_code();
                const auto& member_to_expose = expose_data.class_members[class_id].args_queue.top();
                auto& member = expose_data.class_members[class_id].members[member_to_expose.name];
                const auto member_id = member_to_expose.type_id;
                expose_data.class_members[class_id].args_queue.pop();

                std::vector<size_t> type_ids{};
                if constexpr (sizeof...(Args) != 0) {
                    type_ids.reserve(sizeof...(Args));
                    (type_ids.push_back(typeid(Args).hash_code()), ...);
                }

                for (size_t i = 0; i < types_and_names.size(); i++) {
                    const auto& arg = types_and_names[i];
                    const auto space_pos = arg.find(' ');
                    const auto type = arg.substr(0, space_pos);
                    const auto name = arg.substr(space_pos + 1);
                    auto& sig = (*member.functions)[member_id].signature;
                    sig.arguments.push_back(ExposedClasses::ExposedFunction::Signature::Argument{
                        .type = type,
                        .name = name,
                        .type_id = type_ids.back()
                    });
                }

                return true;
            }

            template<typename Var, std::enable_if_t<!std::is_function_v<Var>, bool> = true>
            static bool expose([[maybe_unused]] Var T::* variable, const std::string& name) {
                auto& expose_data = ExposeApi::get_exposed_classes();
                const size_t class_id = typeid(T).hash_code();

                const auto offset = *reinterpret_cast<uintptr_t*>(&variable);
                expose_data.class_members[class_id].members[name].emplace_variable(offset, name);

                return true;
            }
        };

        template<typename T>
        static ExposeApi expose_class(const std::string& name = "NoName") {
            auto& expose_data = ExposeApi::get_exposed_classes();
            expose_data.class_names[typeid(T).hash_code()] = name;
            return {};
        }
    };

    class ExposeApiRuntime {
    public:
        static size_t get_class_id(ExposeApiRuntime* instance) {
            return typeid(*instance).hash_code();
        }

        struct AutoExposedMemberFinder {
            void* object = nullptr;
            ExposeApi::ExposedClasses::ExposedMember member{};

            template<typename ReturnType = void, typename... Args>
            ReturnType call(Args&&... args) {
                if (!member.has_value || !member.is_function)
                    throw std::runtime_error("Member is not a function");
                auto func = member.functions->at(typeid(ReturnType(Args...)).hash_code());
                auto res = func(object, {std::forward<Args>(args)...});
                if constexpr (!std::is_same_v<ReturnType, void>)
                    return std::any_cast<ReturnType>(res);
            }

            template<typename ReturnType = void, typename... Args>
            ReturnType call(Args&&... args) const {
                if (!member.has_value || !member.is_function)
                    throw std::runtime_error("Member is not a function");
                const auto func = member.functions->at(typeid(ReturnType(Args...)).hash_code());
                const auto res = func(object, {std::forward<Args>(args)...});
                if constexpr (!std::is_same_v<ReturnType, void>)
                    return std::any_cast<ReturnType>(res);
            }

            template<typename M>
            M& get() {
                if (!member.has_value || member.is_function)
                    throw std::runtime_error("Member is not a variable");
                return member.variable->get<M>(object);
            }

            template<typename M>
            const M& get() const {
                if (!member.has_value || member.is_function)
                    throw std::runtime_error("Member is not a variable");
                return member.variable->get_const<M>(object);
            }
            
            ExposeApi::ExposedClasses::ExposedFunction::Signature get_signature() const {
                if (!member.has_value || !member.is_function)
                    throw std::runtime_error("Member is not a function");
                return member.functions->begin()->second.signature;
            }

            bool is_function() const { return member.is_function; }
            bool is_variable() const { return !member.is_function; }
        };
        virtual AutoExposedMemberFinder get_member(const std::string& name) {
            auto& expose_data = ExposeApi::get_exposed_classes();
            const auto type_id = get_class_id(this);
            const auto offset = expose_data.class_members[type_id].runtime_offset;
            const auto _this = reinterpret_cast<uintptr_t>(this) - offset;
            return {
                .object = reinterpret_cast<void*>(_this),
                .member = expose_data.class_members[type_id].members[name]
            };
        }
        virtual std::unordered_multimap<std::string, AutoExposedMemberFinder> get_all_members() {
            std::unordered_multimap<std::string, AutoExposedMemberFinder> members{};
            auto& expose_data = ExposeApi::get_exposed_classes();
            const auto type_id = get_class_id(this);

            for (const auto& [name, member] : expose_data.class_members[type_id].members) {
                if (name == "__expose_api_member_offset_initializer")
                    continue;
                const auto offset = expose_data.class_members[type_id].runtime_offset;
                const auto _this = reinterpret_cast<uintptr_t>(this) - offset;
                members.emplace(name, AutoExposedMemberFinder{
                    .object = reinterpret_cast<void*>(_this),
                    .member = member
                });
            }

            return members;
        }

        virtual ~ExposeApiRuntime() = default;
    };

    template<typename T>
    ExposeApi::ExposedClassAnalyzer<T>::ExposedClassAnalyzer() {
        const auto __offset = &T::__expose_api_analyzer;
        const auto offset = *reinterpret_cast<const uintptr_t*>(&__offset);
        assert(offset == 0 && "Class must not have any members before using the EXPOSE_CLASS() macro");

        const auto __T_instance = (void*)this;
        T* T_instance = *reinterpret_cast<T* const*>(&__T_instance);
        ExposeApiRuntime::AutoExposedMemberFinder empty_member{};
        if constexpr (std::is_base_of_v<ExposeApiRuntime, T>)
            empty_member = T_instance->get_member("__expose_api_member_offset_initializer");
        else
            empty_member = T_instance->static_get_member("__expose_api_member_offset_initializer");
        const auto real_offset = reinterpret_cast<uintptr_t>(empty_member.object) - reinterpret_cast<uintptr_t>(T_instance);
        ExposeApi::get_exposed_classes().class_members[typeid(T).hash_code()].runtime_offset = real_offset;
    }

    #define __STR2(x) #x
    #define __STR(x) __STR2(x)
    #define __CAT2(a, b) a##b
    #define __CAT(a, b) __CAT2(a, b)


    #define MAKE_EMPTY_FUNC_PTR(a) [[maybe_unused]] a


    #define MARGS(...) \
        (__VA_ARGS__); \
        static inline void __CAT(__expose_api_temp_function, __LINE__) (FOR_EACH(MAKE_EMPTY_FUNC_PTR, __VA_ARGS__)) {} \
        static inline const bool __CAT(__expose_api_args_helper_, __LINE__) = mgm::ExposeApi::ExposedClassAnalyzer<__ExposeApi_Self>::expose_args(&__ExposeApi_Self::__CAT(__expose_api_temp_function, __LINE__), {FOR_EACH(__STR, __VA_ARGS__)})
    
    #define MFUNC_NO_AUTO_ARGS(member, ...) \
        static inline* const __expose_api_return_type_##member{}; \
        static inline bool __expose_api_initializer_function_##member() { \
            return mgm::ExposeApi::ExposedClassAnalyzer<__ExposeApi_Self>::expose(&__ExposeApi_Self::member, std::string(#__VA_ARGS__).empty() ? #member : #__VA_ARGS__); \
        } \
        static inline const bool __expose_api_temp_helper_##member = __expose_api_initializer_function_##member(); \
        std::remove_reference_t<std::remove_pointer_t<decltype(__expose_api_return_type_##member)>> member

    #define MFUNC(member, ...) \
        MFUNC_NO_AUTO_ARGS(member, __VA_ARGS__) MARGS
    

    #define MVAR(member, ...) \
        static inline* const __expose_api_var_type_##member{}; \
        static inline bool __expose_api_initializer_var_##member() { \
            return mgm::ExposeApi::ExposedClassAnalyzer<__ExposeApi_Self>::expose(&__ExposeApi_Self::member, std::string(#__VA_ARGS__).empty() ? #member : #__VA_ARGS__); \
        } \
        static inline const bool __expose_api_temp_helper_##member = __expose_api_initializer_var_##member(); \
        std::remove_reference_t<std::remove_pointer_t<decltype(__expose_api_var_type_##member)>> member


    #define EXPOSE_CLASS(class_name) \
        DEFINE_SELF_WITH_NAME(__ExposeApi_Self) \
        __impl_no_unique_address mgm::ExposeApi::ExposedClassAnalyzer<__ExposeApi_Self> __expose_api_analyzer; \
        \
        template<typename __ExposeApi_T = __ExposeApi_Self, std::enable_if_t<!std::is_base_of_v<mgm::ExposeApiRuntime, __ExposeApi_T>, bool> = true> \
        mgm::ExposeApiRuntime::AutoExposedMemberFinder static_get_member(const std::string& name) { \
            return { \
                .object = this, \
                .member = mgm::ExposeApi::get_exposed_classes().class_members[typeid(__ExposeApi_Self).hash_code()].members[name] \
            }; \
        } \
        \
        template<typename __ExposeApi_T = __ExposeApi_Self, std::enable_if_t<!std::is_base_of_v<mgm::ExposeApiRuntime, __ExposeApi_T>, bool> = true> \
        std::unordered_multimap<std::string, mgm::ExposeApiRuntime::AutoExposedMemberFinder> static_get_all_members() { \
            std::unordered_multimap<std::string, mgm::ExposeApiRuntime::AutoExposedMemberFinder> members{}; \
            auto& expose_data = mgm::ExposeApi::get_exposed_classes(); \
            \
            for (const auto& [name, member] : expose_data.class_members[typeid(__ExposeApi_Self).hash_code()].members) { \
                members.emplace(name, mgm::ExposeApiRuntime::AutoExposedMemberFinder{ \
                    .object = this, \
                    .member = member \
                }); \
            } \
            \
            return members; \
        } \
        static inline mgm::ExposeApi __expose_api_class_auto_initializer{mgm::ExposeApi::expose_class<__ExposeApi_Self>(class_name)}; \
        \
        void MFUNC_NO_AUTO_ARGS(__expose_api_member_offset_initializer)() {}
}
