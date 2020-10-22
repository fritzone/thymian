#include "common.h"

#include <boost/tokenizer.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string.hpp>

#include <fpaq0.h>

#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <cassert>
#include <cstring>
#include <cmath>
#include <sstream>
#include <iterator>
#include <cctype>

// Should stay here, do not move before <algorithm> otherwise strange warnings will come
#include <log.h>


const std::string platform()
{
#ifdef __ANDROID__
    return HOSTT_ANDROID;
#elif defined __linux__
    return HOSTT_LINUX;
#elif defined _WIN32
    return HOSTT_WINDOWS;
#else
    return HOSTT_UNKNOWN;
#endif
}

namespace unafrog { namespace utils {

namespace random {

std::string random_string( size_t length, unafrog::utils::random::random_string_class cls )
{
    auto randchar = [cls]() -> char
    {
        auto charset = [cls]() -> std::string {
            switch (cls) {
            case unafrog::utils::random::random_string_class::RSC_DEC:
                return "0123456789";
            case unafrog::utils::random::random_string_class::RSC_HEX:
                return "0123456789abcdef";
            case unafrog::utils::random::random_string_class::RSC_ASC_DEC:
                return "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
            case unafrog::utils::random::random_string_class::RSC_B64:
                return "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ+/";
            case unafrog::utils::random::random_string_class::RSC_FULL:
                return "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ|!#$%&/()=?{[]}+\\-_.:,;'*^";
            }
            return "10";
        }();

        const size_t max_index = (charset.length() - 1);
        return charset[ rand() % max_index ];
    };
    std::string str(length, 0);
    std::generate_n( str.begin(), length, randchar );
    return str;
}

} //random

namespace b62 {

static const char base62_vals[] = "0123456789"
                                  "abcdefghijklmnopqrstuvwxyz"
                                  "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

static const int base62_index[] = {
       0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
       0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
       0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
       0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,    0,    0,
       0,    0,    0,    0,    0, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a,
    0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36,
    0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d,    0,    0,    0,    0,    0,
       0, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14,
    0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20,
    0x21, 0x22, 0x23,
};

void strreverse_inplace (char *str)
{
    char c;
    int half;
    int len;
    int i;

    len = strlen(str);
    half = len >> 1;
    for (i = 0; i < half; i++) {
        c = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = c;
    }
}

std::string base62_encode (uint64_t val)
{
    char str[128] = {0};
    size_t i = 0, len = 128;
    int v;

    assert(str);
    assert(len > 0);

    do {
        if (i + 1 >= len)
            return "";
        v = val % 62;
        str[i++] = base62_vals[v];
        val = (val - v) / 62;
    } while (val > 0);
    str[i] = '\0';
    strreverse_inplace(str);

    return std::string(str);
}

uint64_t base62_decode (const std::string& str)
{
    uint64_t val = 0;
    char c;
    int len;
    int i;

    len = str.length();
    for (i = 0; i < len; i++) {
        c = str[i];
        if (!isalnum(c)) {
            return -1;
        }
        val += base62_index[(int)c] *(uint64_t) powl(62, len - i - 1);
    }

    return val;
}

} // b62

std::string to_upper(const std::string &s)
{
    std::string res = s;
    std::transform(res.begin(), res.end(), res.begin(), ::toupper);
    return res;
}

}} // unafrog::utils

static std::string replace(std::string subject, const std::string& search, const std::string& replace)
{
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos)
    {
         subject.replace(pos, search.length(), replace);
         pos += replace.length();
    }
    return subject;
}

static void encode(std::string& data)
{
    std::string buffer;
    buffer.reserve(data.size());
    for(size_t pos = 0; pos != data.size(); ++pos)
    {
        switch(data[pos])
        {
            case '&':  buffer.append("&amp;");       break;
            case '\"': buffer.append("&quot;");      break;
            case '\'': buffer.append("&apos;");      break;
            case '<':  buffer.append("&lt;");        break;
            case '>':  buffer.append("&gt;");        break;
            default:   buffer.append(&data[pos], 1); break;
        }
    }
    data.swap(buffer);
}

// forward declaration
static int get_number(const std::string& s, size_t &i);

std::string unafrog::utils::sanitize_user_input(const std::string &s, bool remove_domains)
{
    static auto html_tags = {"a", "abbr", "address", "area", "article",
                             "aside", "audio", "b", "base", "bdi", "bdo",
                             "blockquote", "body", "br", "button", "canvas", "caption",
                             "cite", "code", "col", "colgroup", "data", "datalist",
                             "dd", "del", "dfn", "div", "dl", "dt", "em", "embed", "fieldset",
                             "figcaption", "figure", "footer", "form", "h1", "h2", "h3", "h4",
                             "h5", "h6", "head", "header", "hr", "html", "i", "iframe", "img",
                             "input", "ins", "kbd", "keygen", "label", "legend", "li", "link",
                             "main", "map", "mark", "meta", "meter", "nav", "noscript",
                             "object", "ol", "optgroup", "option", "output", "p", "param",
                             "pre", "progress", "q", "rb", "rp", "rt", "rtc", "ruby", "s",
                             "samp", "script", "section", "select", "small", "source", "span",
                             "strong", "style", "sub", "sup", "table", "tbody", "td",
                             "template", "textarea", "tfoot", "th", "thead", "time", "title",
                             "tr", "track", "u", "ul", "var", "video", "wbr", "--#"};

    static auto window_methods_js = {"window.alert", "window.atob", "window.blur", "window.btoa",
                                     "window.clearInterval", "window.clearTimeout", "window.close",
                                     "window.confirm", "window.createPopup", "window.focus", "window.getComputedStyle",
                                     "window.getSelection", "window.matchMedia", "window.moveBy", "window.moveTo",
                                     "window.open", "window.print", "window.prompt", "window.resizeBy",
                                     "window.resizeTo", "window.scroll", "window.scrollBy", "window.scrollTo",
                                     "window.setInterval", "window.setTimeout", "window.stop",
                                     // Should be the last one due to the way the history objects are being accessed
                                     "window." };

    static auto history_methods_js = {"history.back", "history.forward", "history.go"};

    static auto location_js = {"location.hash", "location.host", "location.hostname", "location.href", "location.origin",
                               "location.pathname", "location.port", "location.protocol", "location.search",
                               "location.assign", "location.reload", "location.replace" };

    static auto document_js = {"document.activeElement", "document.addEventListener", "document.adoptNode",
                               "document.anchors", "document.applets", "document.baseURI", "document.body",
                               "document.close", "document.cookie", "document.createAttribute", "document.createComment",
                               "document.createDocumentFragment", "document.createElement", "document.createTextNode",
                               "document.doctype", "document.documentElement", "document.documentMode",
                               "document.documentURI", "document.domain", "document.domConfig", "document.embeds",
                               "document.forms", "document.getElementById", "document.getElementsByClassName",
                               "document.getElementsByName", "document.getElementsByTagName", "document.hasFocus",
                               "document.head", "document.images", "document.implementation", "document.importNode",
                               "document.inputEncoding", "document.lastModified", "document.links",
                               "document.normalize", "document.normalizeDocument", "document.open", "document.querySelector",
                               "document.querySelectorAll", "document.readyState", "document.referrer",
                               "document.removeEventListener", "document.renameNode", "document.scripts",
                               "document.strictErrorChecking", "document.title", "document.URL", "document.write",
                               "document.writeln"};

    static auto js_events = {"onclick", "oncontextmenu", "ondblclick", "onmousedown", "onmouseenter", "onmouseleave",
                          "onmousemove", "onmouseover", "onmouseout", "onmouseup", "onkeydown", "onkeypress", "onkeyup",
                          "onabort", "onbeforeunload", "onerror", "onhashchange", "onload", "onpageshow", "onpagehide",
                          "onresize", "onscroll", "onunload", "onblur", "onchange", "onfocus", "onfocusin", "onfocusout",
                          "oninput", "oninvalid", "onreset", "onsearch", "onselect", "onsubmit", "ondrag", "ondragend",
                          "ondragenter", "ondragleave", "ondragover", "ondragstart", "ondrop", "oncopy", "oncut",
                          "onpaste", "onafterprint", "onbeforeprint", "onabort", "oncanplay", "oncanplaythrough",
                          "ondurationchange", "onemptied", "onended", "onerror", "onloadeddata", "onloadedmetadata",
                          "onloadstart", "onpause", "onplay", "onplaying", "onprogress", "onratechange", "onseeked",
                          "onseeking", "onstalled", "onsuspend", "ontimeupdate", "onvolumechange", "onwaiting",
                          "animationend", "animationiteration", "animationstart", "transitionend", "onerror",
                          "onmessage", "onopen", "onmessage", "onmousewheel", "ononline", "onoffline", "onpopstate",
                          "onshow", "onstorage", "ontoggle", "onwheel", "ontouchcancel", "ontouchend", "ontouchmove",
                          "ontouchstart", "cancelable", "currentTarget", "defaultPrevented", "eventPhase",
                          "isTrusted", "timeStamp", "preventDefault", "stopImmediatePropagation", "stopPropagation",
                          "altKey", "clientX", "clientY", "ctrlKey", "metaKey", "pageX", "pageY", "relatedTarget",
                          "screenX", "screenY", "shiftKey", "altKey", "ctrlKey", "charCode", "keyCode",
                          "metaKey", "shiftKey", "newURL", "oldURL", "relatedTarget", "animationName", "elapsedTime",
                          "propertyName", "elapsedTime", "deltaX", "deltaY", "deltaZ", "deltaMode" };

    static auto js_globals = {"decodeURI","decodeURIComponent", "encodeURI", "encodeURIComponent",
                              "eval", "isFinite", "isNaN", "Number", "parseFloat", "parseInt", "String", "unescape" };

    static auto js_navigator = {"appCodeName", "appName", "appVersion", "cookieEnabled",
                                "geolocation", "onLine", "userAgent" };

    static auto toplevel_domains = {".academy",".accountant",".accountants",".cloud",".active",".actor",".adult",".aero",".agency",".airforce",
                ".apartments",".app",".archi",".army",".associates",".attorney",".auction",".audio",".autos",".band",".bar",
                ".bargains",".beer",".best",".bid",".bike",".bingo",".bio",".biz",".black",".blackfriday",".blog",".blue",
                ".boo",".boutique",".build",".builders",".business",".buzz",".cab",".camera",".camp",".cancerresearch",
                ".capital",".cards",".care",".career",".careers",".cash",".casino",".catering",".center",".ceo",".channel",
                ".chat",".cheap",".christmas",".church",".city",".claims",".cleaning",".click",".clinic",".clothing",
                ".club",".coach",".codes",".coffee",".college",".community",".company",".computer",".condos",".construction",
                ".consulting",".contractors",".cooking",".cool",".coop",".country",".coupons",".credit",".creditcard",
                ".cricket",".cruises",".dad",".dance",".date",".dating",".day",".deals",".degree",".delivery",".democrat",
                ".dental",".dentist",".design",".diamonds",".diet",".digital",".direct",".directory",".discount",".dog",
                ".domains",".download",".eat",".education",".email",".energy",".engineer",".engineering",".equipment",
                ".esq",".estate",".events",".exchange",".expert",".exposed",".express",".fail",".faith",".family",
                ".fans",".farm",".fashion",".pid",".finance",".financial",".fish",".fishing",".fit",".fitness",".flights",
                ".florist",".flowers",".fly",".foo",".football",".forsale",".foundation",".fund",".furniture",".fyi",
                ".gallery",".garden",".gift",".gifts",".gives",".glass",".global",".gold",".golf",".gop",".graphics",
                ".green",".gripe",".guide",".guitars",".guru",".healthcare",".help",".here",".hiphop",".hiv",".hockey",
                ".holdings",".holiday",".homes",".horse",".host",".hosting",".house",".how",".info",".ing",".ink",
                ".institute[59]",".insure",".international",".investments",".jewelry",".jobs",".kim",".kitchen",".land",
                ".lawyer",".lease",".legal",".lgbt",".life",".lighting",".limited",".limo",".link",".loan",".loans",
                ".lol",".lotto",".love",".luxe",".luxury",".management",".market",".marketing",".markets",".mba",".media",
                ".meet",".meme",".memorial",".men",".menu",".mobi",".moe",".money",".mortgage",".motorcycles",".mov",
                ".movie",".museum",".name",".navy",".network",".new",".news",".ngo",".ninja",".one",".ong",".onl",
                ".online",".ooo",".organic",".partners",".parts",".party",".pharmacy",".photo",".photography",".photos",
                ".physio",".pics",".pictures",".feedback",".pink",".pizza",".place",".plumbing",".plus",".poker",".porn",
                ".post",".press",".pro",".productions",".prof",".properties",".property",".qpon",".racing",".recipes",
                ".red",".rehab",".ren",".rent",".rentals",".repair",".report",".republican",".rest",".review",".reviews",
                ".rich",".rip",".rocks",".rodeo",".rsvp",".run",".sale",".school",".science",".services",".sex",".sexy",
                ".shoes",".show",".singles",".site",".soccer",".social",".software",".solar",".solutions",".space",
                ".studio",".style",".sucks",".supplies",".supply",".support",".surf",".surgery",".systems",".tattoo",
                ".tax",".taxi",".team",".tech",".technology",".tel",".tennis",".theater",".tips",".tires",".today",
                ".tools",".top",".tours",".town",".toys",".trade",".training",".travel",".university",".vacations",
                ".vet",".video",".villas",".vision",".vodka",".vote",".voting",".voyage",".wang",".watch",".webcam",
                ".website",".wed",".wedding",".whoswho",".wiki",".win",".wine",".work",".works",".world",".wtf",
                ".xxx",".xyz",".yoga",".zone",".maison",".abogado",".gratis",".futbol",".juegos",".soy",".tienda",
                ".uno",".viajes",".haus",".immobilien",".jetzt",".kaufen",".reise",".reisen",".schule",".versicherung",
                ".desi",".shiksha",".casa",".cafe",".immo",".moda",".voto",".bar",".bank",".coop",".enterprises",
                ".industries",".institute",".ltda",".pub",".realtor",".reit",".rest",".restaurant",".sarl",".ventures",
                ".capetown",".durban",".joburg",".asia",".krd",".nagoya",".okinawa",".ryukyu",".taipei",".tatar",".tokyo",
                ".yokohama",".alsace",".amsterdam",".barcelona",".bayern",".berlin",".brussels",".budapest",".bzh",
                ".cat",".cologne",".corsica",".cymru",".eus",".frl",".gal",".gent",".hamburg",".irish",".koeln",".london",
                ".madrid",".moscow",".nrw",".paris",".ruhr",".saarland",".scot",".tirol",".vlaanderen",".wales",".wien",
                ".zuerich",".miami",".nyc",".quebec",".vegas",".kiwi",".melbourne",".sydney",".lat",".rio",".allfinanz",
                ".android",".aquarelle",".axa",".barclays",".barclaycard",".bloomberg",".bmw",".bnl",".bnpparibas",".cal",
                ".caravan",".cern",".chrome",".citic",".crs",".cuisinella",".dnp",".dvag",".emerck",".everbank",".firmdale",
                ".flsmidth",".frogans",".gbiz",".gle",".globo",".gmail",".gmo",".gmx",".google",".hsbc",".ibm",".kred",
                ".lacaixa",".latrobe",".lds",".mango",".mini",".monash",".mormon",".neustar",".nexus",".nhk",".nico",".nra",
                ".otsuka",".ovh",".piaget",".pohl",".praxi",".prod",".pwc",".sandvikcoromant",".sca",".scb",".schmidt",".sohu",
                ".spiegel",".suzuki",".tui",".uol",".williamhill",".wme",".wtc",".yandex",".youtube",".com",".org",".net",
                ".int",".edu",".gov",".mil",".arpa",".ac",".ad",".ae",".af",".ag",".ai",".al",".am",".an",".ao",".aq",".ar",
                ".as",".at",".au",".aw",".ax",".az",".ba",".bb",".bd",".be",".bf",".bg",".bh",".bi",".bj",".bm",".bn",".bo",
                ".bq",".br",".bs",".bt",".bv",".bw",".by",".bz",".ca",".cc",".cd",".cf",".cg",".ch",".ci",".ck",".cl",".cm",
                ".cn",".co",".cr",".cu",".cv",".cw",".cx",".cy",".cz",".de",".dj",".dk",".dm",".do",".dz",".ec",".ee",".eg",
                ".eh",".er",".es",".et",".eu",".fi",".fj",".fk",".fm",".fo",".fr",".ga",".gb",".gd",".ge",".gf",".gg",".gh",
                ".gi",".gl",".gm",".gn",".gp",".gq",".gr",".gs",".gt",".gu",".gw",".gy",".hk",".hm",".hn",".hr",".ht",".hu",
                ".id",".ie",".il",".im",".in",".io",".iq",".ir",".is",".it",".je",".jm",".jo",".jp",".ke",".kg",".kh",".ki",
                ".km",".kn",".kp",".kr",".kw",".ky",".kz",".la",".lb",".lc",".li",".lk",".lr",".ls",".lt",".lu",".lv",".ly",
                ".ma",".mc",".md",".me",".mg",".mh",".mk",".ml",".mm",".mn",".mo",".mp",".mq",".mr",".ms",".mt",".mu",".mv",
                ".mw",".mx",".my",".mz",".na",".nc",".ne",".nf",".ng",".ni",".nl",".no",".np",".nr",".nu",".nz",".om",".pa",
                ".pe",".pf",".pg",".ph",".pk",".pl",".pm",".pn",".pr",".ps",".pt",".pw",".py",".qa",".re",".ro",".rs",".ru",
                ".rw",".sa",".sb",".sc",".sd",".se",".sg",".authenticator.cloudy.sh",".si",".sj",".sk",".sl",".sm",".sn",".so",".sr",".ss",".st",
                ".su",".sv",".sx",".sy",".sz",".tc",".td",".tf",".tg",".th",".tj",".tk",".tl",".tm",".tn",".to",".tp",".tr",
                ".tt",".tv",".tw",".tz",".ua",".ug",".uk",".us",".uy",".uz",".va",".vc",".ve",".vg",".vi",".vn",".vu",".wf",
                ".ws",".ye",".yt",".za",".zm",".zw"};


    static auto with_domains = {window_methods_js, history_methods_js, location_js, document_js, js_events, js_globals, js_navigator,toplevel_domains};
    static auto without_domains = {window_methods_js, history_methods_js, location_js, document_js, js_events, js_globals, js_navigator};
    static auto containers = remove_domains ? with_domains : without_domains;

    std::string result = s;

    // First run: HTML tags
    for(auto tag : html_tags)
    {

        // Ddi we parse out all the garbage?
        if(result.empty())
        {
            break;
        }

        // Zero: Standalone tags
        std::string open_tag = std::string("<") + tag + std::string(">");
        result = replace(result, open_tag, "");
        std::string close_tag = std::string("</") + tag + std::string(">");
        result = replace(result, close_tag, "");

        // One: Tags which might have parameters, such as: <script language = "blabl">
        // Involves parsing out the entire tag

        std::string endless_tag = std::string("<") + tag;
        std::size_t etpos = result.find(endless_tag);
        if( etpos != std::string::npos)
        {
            std::string tmps = result.substr(etpos);
            size_t i = 0;   // i will point to the first character after the endless tag
            while(i < tmps.length())
            {
                // skip stuff on double quotes
                if(tmps[i] == '\"')
                {
                    i ++;
                    while(i < tmps.length())
                    {
                        // skip the escaped double quotes
                        if(tmps[i] == '\\') i++;
                        if(i < tmps.length() && tmps[i] == '\"') i++;

                        // advance
                        if(i < tmps.length() ) i++;
                    }
                }

                if( i < tmps.length() )
                {
                    if(tmps[i] == '>')
                    {
                        break;
                    }
                    i++;
                }
            }

            i++;

            if( i<tmps.length())
            {
                result = tmps.substr(i);
            }
            else
            {
                result.clear();
            }
        }
    }

    // Second run: various javascript stuff
    for(auto container : containers)
    {
        for(auto tag : container)
        {
            result = replace(result, tag, "");
        }
    }

again:
    size_t i = 0;
    while(i < result.length())
    {
        // the first part
        if(isdigit(result[i]))
        {
            size_t save_i = i;
            int p1 = get_number(result, i);
            if(p1 == 2)
            {
                // skip the '.'
                i++;
                // get the next number
                int p2 = get_number(result, i);
                if(p2 == 2)
                {
                    i++;
                    int p3 = get_number(result, i);
                    if(p3 == 2)
                    {
                        // skip '.'
                        i++;
                        int p4 = get_number(result, i);
                        if(p4 == 1)
                        {
                            // now remove from result the section between save_i and i
                            result = result.substr(0, save_i) + result.substr(i);
                            goto again;
                        }
                    }
                }
            }
            else
            {
                continue;
            }

        }
        else
        {
            // move to the next character
            i++;
        }
    }

    // Third run: Encode the remaining XML/HTML artifacts into web-safe data
    encode(result);
    return result;
}

// returns: 0 - not a valid IP number, 1 - is not followed by '.', 2 - valid IP number, followed by '.'
static int get_number(const std::string& s, size_t& i)
{
    std::string ip1 = "";
    while(isdigit(s[i]) && i < s.length())
    {
        ip1 += s[i++];
    }

    // is this a valid IP part?
    int nip1 = atoi(ip1.c_str());
    if(nip1 > 255)
    {
        return 0;
    }

    if(s[i] == '.')
    {
        return 2;
    }
    return 1;
}

static bool case_insensitive_eq(const char &lhs, const char &rhs)
{
    return std::toupper(lhs) == std::toupper(rhs);
}

static bool case_insensitive_string_eq(const std::string &lhs, const std::string &rhs)
{
    return std::equal(lhs.begin(),
                      lhs.end(),
                      rhs.begin(),
                      case_insensitive_eq);
}

case_insensitive_str_eq::case_insensitive_str_eq(std::string key) : key_(std::move(key)) {}

bool case_insensitive_str_eq::operator()(const std::string &item) const
{
    if(item.length() != key_.length())
    {
        return false;
    }
    return case_insensitive_string_eq(item, key_);
}

std::string unafrog::server()
{
   return "http://localhost:8838";
}

std::string unafrog::utils::consume(std::string& s, int c)
{
    try
    {
        std::string v = s.substr(0, c);
        s.erase(0, c);
        return v;
    }
    catch (std::exception& ex)
    {
        log_err() << "Cannot consume" << c << "characters from" << s << ": " << ex.what();
        return "";
    }
    catch(...)
    {
        std::stringstream ss;
        ss << "Cannot consume" << c << "characters from" << s;
        log_err() << ss.str();
        return "";
    }
}

std::vector<uint8_t> unafrog::utils::hex_string_to_vector(const std::string &s)
{
    std::vector<uint8_t> out;
    for (unsigned int i = 0; i < s.length(); i += 2)
    {
      std::string byteString = s.substr(i, 2);
      uint8_t byte = static_cast<uint8_t>(strtol(byteString.c_str(), NULL, 16));
      out.push_back(byte);
    }
    return out;
}

std::string unafrog::utils::string_to_hex(const std::string& input)
{
    static const char* const lut = "0123456789abcdef";
    size_t len = input.length();

    std::string output;
    output.reserve(2 * len);
    for (size_t i = 0; i < len; ++i)
    {
        const unsigned char c = input[i];
        output.push_back(lut[c >> 4]);
        output.push_back(lut[c & 15]);
    }
    return output;
}

std::string unafrog::utils::hex_to_string(const std::string& input)
{
    static const char* const lut = "0123456789abcdef";
    size_t len = input.length();
    if (len & 1)
    {
        log_err() << "odd length for " + input;
        throw std::exception();
    }

    std::string output;
    output.reserve(len / 2);
    for (size_t i = 0; i < len; i += 2)
    {
        char a = input[i];
        const char* p = std::lower_bound(lut, lut + 16, a);
        if (*p != a)
        {
            log_err() << "not a hex digit";
            throw std::exception();
        }

        char b = input[i + 1];
        const char* q = std::lower_bound(lut, lut + 16, b);
        if (*q != b)
        {
            log_err() << "not a hex digit in: " + input;
            throw std::exception();
        }

        output.push_back(((p - lut) << 4) | (q - lut));
    }
    return output;
}

std::string unafrog::utils::grow(const std::string &s, std::size_t required_length)
{
    std::string result = s;
    while(result.length() < required_length)
    {
        result = "0" + result;
    }
    return result;
}

std::string join(const std::vector<std::string> &vec, const char *delim)
{
    std::stringstream res;
    std::copy(vec.begin(), vec.end(), std::ostream_iterator<std::string>(res, delim));
    return res.str();
}

#if !defined __ANDROID__
std::vector<std::string> split(const std::string &s, const char* delim)
{
    std::vector<std::string> result;
    boost::char_separator<char> sep(delim);
    boost::tokenizer<boost::char_separator<char>> tok(s, sep);

    for(boost::tokenizer<boost::char_separator<char>>::iterator beg=tok.begin(); beg!=tok.end(); ++beg)
    {
        result.push_back(*beg);
    }

    return result;
}
#endif

unsigned internalServerError()
{
    return 500; // HTTP_INTERNAL_SERVER_ERROR;
}

std::string remove_duplicates(std::string s, char to_remove)
{
    std::string result;
    auto i = s.begin();
    while(i != s.end())
    {
        auto pv = i;
        result += *i ++;
        while(i!=s.end() && *i == *pv && *i == to_remove)
        {
            ++ i;
        }
    }

    return result;
}

std::string unafrog::utils::sanitize_hostname_web(std::string hn)
{
    std::vector<std::pair<char, char>> replacements{
        { '-', '_' },
        { '.', '_' }
    };

    for (auto const &r : replacements) {
        hn.replace(hn.begin(), hn.end(), r.first, r.second);
    }

    return hn;
}

void remove_quotes(std::string &s)
{
    boost::trim_if(s, boost::is_any_of("\""));
}
