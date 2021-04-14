#include "category_sender.h"
#include "templater.h"

#include <tntdb.h>

category_sender::category_sender(tnt::HttpRequest &request, tnt::HttpReply &reply, const std::string &what) : web_component(request, reply, "")
{
	try {
		tntdb::Connection conn = tntdb::connect("sqlite:lang.db");

		// TODO: prepared statement

		std::string v = std::string("select * from food where type='") + "0" + "'";
		tntdb::Result result = conn.select(v);
		for (tntdb::Result::const_iterator it = result.begin(); it != result.end(); ++it)
		{
			std::string name_src = "";
			std::string img = "";
			std::string desc = "";
			int food_idx = -1;
			std::string type;

			tntdb::Row row = *it;

			row[0].get(food_idx);
			row[1].get(name_src);
			row[2].get(type);
			row[3].get(img);
			row[4].get(desc);

			template_struct food("foods", "fooditem");

			food["name"] = name_src;
			food["img"] = img;
			food["desc"] = desc;

			structs.push_back(food);

			std::cerr << name_src;
		}
	}
	catch (std::exception& ex)
	{
		std::cerr << ex.what();
	}

}

unsigned category_sender::send()
{
	template_vector_par tvp("foods", structs);

	std::string page = templater<category_list>().templatize(tvp).get();
	mreply.out() << page;

	return HTTP_OK;
}
