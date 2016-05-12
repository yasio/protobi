#ifndef _EVENT_HANDLER_MANAGER_H_
#define _EVENT_HANDLER_MANAGER_H_
#include <map>
#include <set>
#include <unordered_map>
#include "container_helper.h"

namespace purelib {

namespace privacy{ // reusable event handler manager templates

struct event_handler_wrapper_base
{
    virtual int get_priority() const = 0;
    virtual int get_order() const = 0;
};

inline bool operator<(const event_handler_wrapper_base& lhs, const event_handler_wrapper_base& rhs)
{
    return lhs.get_priority() < rhs.get_priority() ||
        (lhs.get_priority() == rhs.get_priority() && lhs.get_order() < rhs.get_order());
}

template<typename _EventSender, typename _EventId, typename _EventType, typename _EventArg>
class event_handler_manager
{
public:
    class event_handler
    {
        friend class event_handler_manager;
       
    public:
        virtual bool invoke(_EventSender sender,  _EventArg) = 0;

        void set_event_type(_EventType t) { type = t; }
        _EventType get_event_type(void) const { return type; }

    private:
        _EventId eid;
        _EventType type;
        bool enabled = true;
        int priority = 0;
        int order = 0;
    };

    struct event_handler_wrapper : public event_handler_wrapper_base
    {
        event_handler_wrapper(event_handler* ins) : handler(ins){}
        virtual int get_priority() const override { return handler->priority; }
        virtual int get_order() const override { return handler->order; }
        event_handler* handler;
    };

public:
    ~event_handler_manager(void)
    {
        container_helper::clear_cc(this->ec, [](std::set<event_handler_wrapper>& ehw_set) {
            for (auto& ehw : ehw_set)
                delete ehw.handler;
            ehw_set.clear();
        });
    }

    event_handler* register_handler(const _EventId& eid, event_handler* eh)
    {
        static const std::set<event_handler_wrapper> nil_eh_set;
        auto target_ehpr = this->ec.find(eid);
        if(this->ec.end() == target_ehpr)
        {
            target_ehpr = this->ec.insert(std::make_pair(eid, nil_eh_set)).first;
        }

        eh->eid = eid;
        eh->order = target_ehpr->second.size();

        target_ehpr->second.insert(event_handler_wrapper( eh ));
        return eh;
    }

    void unregister_handler(event_handler* specific_handler)
    {
        auto deleter = this->ec.find(specific_handler->eid);
        if(deleter != this->ec.end())
        {
            auto ehiter = deleter->second.find(event_handler_wrapper( specific_handler ));
            if(ehiter != deleter->second.end())
            {
                deleter->second.erase(ehiter);
                delete specific_handler;
            }
        }
    }

    std::set<event_handler_wrapper>* get_handler_set(const _EventId& eid)
    {
        auto target = this->ec.find(eid);
        if (target != this->ec.end())
        {
            return &target->second;
        }
        return nullptr;
    }

    void unregister_handler(const _EventId& eid)
    {
        auto deleter = this->ec.find(eid);
        if(deleter != this->ec.end())
        {
            for (auto& ehw : deleter->second)
                delete ehw.handler;
            deleter->second.clear();
        }
    }

    void modify_priority(event_handler* handler, int priority)
    {
        auto modifier = this->ec.find(handler->eid);
        if (modifier != this->ec.end())
        {
            auto target = modifier->second.find(event_handler_wrapper(handler));
            if (target != modifier->second.end())
            {
                if (handler->priority != priority) {
                    modifier->second.erase(target);
                    handler->priority = priority;
                    modifier->second.insert(event_handler_wrapper(handler));
                }
            }
        }
    }

    bool invoke(const _EventId& eid, _EventSender sender, _EventArg arg)
    {
		auto invoke_pr = this->ec.find(eid);
        if(invoke_pr != this->ec.end())
        {
            for (auto ehw : invoke_pr->second)
            {
                if (ehw.handler->enabled && ehw.handler->invoke(sender, arg))
                {
                    return true;
                }
            }
        }
		return false;
    }

private:

    template<typename _Fty, int _ArgFlags = 2>
    class event_handler_impl : public event_handler
    {
    public:
        event_handler_impl(const _Fty& _Func) : callee(_Func)
        {
        }
        virtual bool invoke(_EventSender sender,  _EventArg arg)
        {
            return this->callee(sender, arg);
        }

    private:
        _Fty callee;
    };

    template<typename _Fty>
    class event_handler_impl<_Fty, 1> : public event_handler
    {
    public:
        event_handler_impl(const _Fty& _Func) : callee(_Func)
        {
        }
        virtual bool invoke(_EventSender,  _EventArg arg)
        {
            return this->callee(arg);
        }

    private:
        _Fty callee;
    };

    template<typename _Fty>
    class event_handler_impl<_Fty, 0> : public event_handler
    {
    public:
        event_handler_impl(const _Fty& _Func) : callee(_Func)
        {
        }

        virtual bool invoke(_EventSender,  _EventArg)
        {
            return this->callee();
        }

    private:
        _Fty callee;
    };

    template<typename _Fty>
    struct event_handler_factory
    {
        static event_handler* create(const _Fty& _Func)
        {
            return create_event_handler(_Func, &_Fty::operator());
        }

    private:
        static event_handler* create_event_handler(const _Fty& _Func, bool (_Fty::*)(_EventSender, _EventArg))
        {
            return (new event_handler_impl<_Fty>(_Func));
        }

        static event_handler* create_event_handler(const _Fty& _Func, bool (_Fty::*)(_EventSender, _EventArg) const)
        {
            return (new event_handler_impl<_Fty, 2>(_Func));
        }

        static event_handler* create_event_handler(const _Fty& _Func, bool (_Fty::*)(_EventArg))
        {
            return (new event_handler_impl<_Fty, 1>(fo));
        }

        static event_handler* create_event_handler(const _Fty& _Func, bool (_Fty::*)(_EventArg) const)
        {
            return (new event_handler_impl<_Fty, 1>(_Func));
        }

        static event_handler* create_event_handler(const _Fty& _Func, bool (_Fty::*)(void))
        {
            return (new event_handler_impl<_Fty, 0>(_Func));
        }

        static event_handler* create_event_handler(const _Fty& _Func, bool (_Fty::*)(void) const)
        {
            return (new event_handler_impl<_Fty, 0>(_Func));
        }
    };

    template<typename _Return>
    class event_handler_factory<_Return (*)(_EventSender, _EventArg)>
    {
    public:
        static event_handler* create(_Return (*pf)(_EventSender, _EventArg))
        {
            return (new event_handler_impl<_Return (*)(_EventSender, _EventArg), 2>(pf));
        }
    };

    template<typename _Return>
    class event_handler_factory<_Return (*)( _EventArg)>
    {
    public:
        static event_handler* create(_Return (*pf)( _EventArg))
        {
            return (new event_handler_impl<_Return (*)( _EventArg), 1>(pf));
        }
    };

    template<typename _Return>
    class event_handler_factory<_Return (*)(void)>
    {
    public:
        static event_handler* create(_Return (*pf)(void))
        {
            return (new event_handler_impl<_Return (*)(void), 0>(pf));
        }
    };

public:
    /// new a event handler
    template<typename _Fty>
    static event_handler* new_event_handler(const _Fty& handler_function)
    {
        return event_handler_factory<_Fty>::create(handler_function);
    }

private:
    std::unordered_map<_EventId, std::set<event_handler_wrapper> > ec;
};

}; // namespace: purelib::privacy
}; // namespace: purelib

#endif
/*
* Copyright (c) 2012-2015 by X.D. Guo  ALL RIGHTS RESERVED.
* Consult your license regarding permissions and restrictions.
**/

