#include <vmime/vmime.hpp>
#include <json.h>
#include <templater.h>

#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <streambuf>

void send_message(const std::string& destination, const std::string& subject, const std::string& body)
{
    std::stringstream emailBodySS;
    emailBodySS << body;

    // create the email message as per vmime's instructions
    vmime::messageBuilder mb;
    mb.setSubject(vmime::text(subject));
    mb.setExpeditor(vmime::mailbox("no-reply@cloudy.sh"));
    mb.getRecipients().appendAddress(vmime::make_shared<vmime::mailbox>(destination));
    mb.getTextPart()->setCharset(vmime::charsets::ISO8859_15) ;
    mb.getTextPart()->setText(vmime::make_shared<vmime::stringContentHandler>(emailBodySS.str()));

    // Message construction
    vmime::shared_ptr<vmime::message> msg = mb.construct();

    // Output raw message data to a stringstream
    std::stringstream ss;
    vmime::utility::outputStreamAdapter out(ss) ;
    msg->generate(out);
    std::string s = ss.str();

    vmime::utility::url url("smtp://localhost");
    vmime::shared_ptr<vmime::net::session> sess = vmime::make_shared<vmime::net::session>();
    vmime::shared_ptr<vmime::net::transport> tr = sess->getTransport(url) ;
    tr->connect();
    vmime::utility::inputStreamStringAdapter issa(s) ;

    vmime::mailbox from("no-reply@cloudy.sh") ;
    vmime::mailboxList to;
    to.appendMailbox(vmime::make_shared<vmime::mailbox>(destination) ) ;

    // send the email
    tr->send( from, to, issa, s.length ( ) ) ;
    tr->disconnect () ;

}

int main()
{
    using nlohmann::json;

    std::ifstream i("dest.json");
    json j;
    i >> j;

    std::cout << "Subject:" << j["mail"]["subject"] << std::endl;
    std::cout << "Body" << j["mail"]["body"] << std::endl;


    std::ifstream t(j["mail"]["body"]);

    if(!t)
    {
        std::cerr << "Invalid body file" << std::endl;
        return 1;
    }

    std::string str((std::istreambuf_iterator<char>(t)),
                     std::istreambuf_iterator<char>());

    STRING_VAR_TEMPLATE(EmailBody, str);


    for(const auto& t : j["mail"]["targets"])
    {
        std::string to = t["to"];
        to.erase(remove( to.begin(), to.end(), '\"' ), to.end());

        send_message(t["address"], j["mail"]["subject"], templater<EmailBody>().templatize("target" <is> to).get());
        std::cout << "To: " << t["address"] << std::endl << templater<EmailBody>().templatize("target" <is> t["to"]).get() << std::endl
                  << "----------------------------------"<< std::endl;
    }

}
