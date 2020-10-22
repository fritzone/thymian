#ifndef STRFY_H
#define STRFY_H

#include <QString>
#include <string>
#include <vector>
#include <map>

inline QString std2q(const std::string & s)
{
    return QString::fromStdString(s);
}

inline std::string q2std(const QString& a)
{
    return a.toStdString();
}

inline QString sanitize_hostname(QString hn)
{
    std::vector<std::pair<QString, QString>> replacements{
        { "-", "_" }
    };

    for (auto const &r : replacements) {
        hn.replace(r.first, r.second);
    }

    return hn;
}

#endif // STRFY_H
