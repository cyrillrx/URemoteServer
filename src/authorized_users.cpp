#include "authorized_users.h"

authorized_users::authorized_users(const std::string &path) : properties(path) { }

authorized_users::~authorized_users() { }

bool authorized_users::is_authorized(const std::string &key)
{
    try {
        get_string(key);
        return true;

    } catch (const std::exception &) {
        return false;
    }
}

std::string authorized_users::get_user(const std::string &key)
{
    return get_string(key);
}