#include "utils.h"

using namespace std;

/*
#include <vector>
#include <locale>
#include <codecvt>

inline string toUtf(const string & gb2312) {
    const static locale loc("zh_CN.GB18030");

    vector<wchar_t> wstr(gb2312.size());
    wchar_t *       wstrEnd = nullptr;
    const char *    gbEnd   = nullptr;
    mbstate_t       state   = {};
    int             res =
        use_facet<codecvt<wchar_t, char, mbstate_t>>(loc).in(state,
                                                             gb2312.data(),
                                                             gb2312.data() + gb2312.size(),
                                                             gbEnd,
                                                             wstr.data(),
                                                             wstr.data() + wstr.size(),
                                                             wstrEnd);

    if (codecvt_base::ok == res) {
        wstring_convert<codecvt_utf8<wchar_t>> cutf8;
        return cutf8.to_bytes(wstring(wstr.data(), wstrEnd));
    }

    return string();
}
*/
