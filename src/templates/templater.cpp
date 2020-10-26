#include "templater.h"

#ifdef PYTHON_SCRIPTING
#include "pyembed.h"
#endif

#include <algorithm>

#include <log.h>
#include <cwctype>

std::string templater_base::get(const std::string &template_name)
{
    // Getting the content
    std::string content = template_warehouse::instance().getTemplateContent(template_name);

    // Resolve the #define's. This will update the extra_kp's
    std::string content_without_vars = resolve_defines(content);

    // resolve all the top level template arguments
    std::string templatized = stringholder(content_without_vars).templatize(kps).get();

    // then search for all the {#include 's and: templatize those with the given variable: Value pairs
    // and include them here
    bool done = false;
    while(!done)
    {
        // search for structure definitions, there can be more than one
        size_t struct_pos = templatized.find(struct_tag);
        while(struct_pos != std::string::npos)
        {
            templatized = resolve_structure_declaration(struct_pos, templatized);
            struct_pos = templatized.find(struct_tag);
        }

        // search for input data definition hints
        size_t parameters_pos = templatized.find(in_tag);
        if(parameters_pos != std::string::npos)
        {
            templatized = resolve_parameters(parameters_pos, templatized);
        }

        // search for the "include"'s
        size_t inc_pos = templatized.find(include_tag);
        while(inc_pos != std::string::npos)
        {
            templatized = resolve_includes(inc_pos, templatized);

            // see if we have pulled in some extra vars
            templatized = stringholder(templatized).templatize(kps).get();

            inc_pos = templatized.find(include_tag);
        }

        // now search for the "if"'s. Remember, ifs cannot have ifs in their body
        size_t if_pos = templatized.find(if_tag);
        if(if_pos != std::string::npos)
        {
            while(if_pos != std::string::npos)
            {
                templatized = resolve_ifs(if_pos, templatized);
                if_pos = templatized.find(if_tag);
            }
        }
        else
        {
            done = true;
        }
    }

    return templatized;
}

std::string templater_base::extract_identifier_word(const std::string& input, size_t& i, std::vector<char> separators,
                                                    std::set<char> extra_allowed_chars, char&& c)
{
    skip_whitespace(input, i);

    std::string result = "";
    while(i < input.length() && (input.at(i) == '_' || extra_allowed_chars.count(input.at(i)) || isalnum(input.at(i))) )
    {
        if(separators.empty())
        {
            result += input.at(i);
            i++;
        }
        else
        {
            if(find(separators.begin(), separators.end(), input.at(i)) == separators.end())
            {
                result += input.at(i);
                i++;
            }
            else
            {
                if(c != 0)
                {
                    c = input[i];
                }
                i ++; // skip the actual separator
                break;
            }
        }
    }

    skip_whitespace(input, i);
    if(!separators.empty())
    {
        if(find(separators.begin(), separators.end(), input.at(i)) != separators.end())
        {
            if(c != 0)
            {
                c = input[i];
            }
            i++; // this characater is a separator. Skip it. And the space after
            skip_whitespace(input, i);
        }
    }

    return result;
}

bool templater_base::check_opening_parenthesis(const std::string& input, size_t& i)
{
    bool opening_parentheses = false;
    // skip the parenthesis if any
    if(i < input.length() && input.at(i) == '(')
    {
        i ++;
        skip_whitespace(input, i);
        opening_parentheses = true;
    }

    return opening_parentheses;
}

bool templater_base::check_closing_comment(const std::string& templatized, size_t& i, std::size_t& include_tag_end_position)
{
    if(i + 3 < templatized.length())
    {
        if(templatized[i+1] == '-' && templatized[i+2] == '-' && templatized[i + 3] == '>')
        {
            include_tag_end_position = i + 4;
            return true;
        }
    }
    return false;
}

std::string templater_base::get_error() const
{
    return error;
}

std::vector<std::string> templater_base::variables()
{
    std::vector<std::string> result;

    /*std::string content = template_warehouse::instance().getTemplateContent(template_name);

    while((start_pos = str.find(from, start_pos)) != std::string::npos)
    {*/

    return result;
}

std::string templater_base::resolve_includes(size_t inc_pos, std::string templatized)
{
    std::map<std::string, std::string> local_pairs;
    std::size_t include_tag_end_position = std::string::npos;
    std::size_t include_tag_start_position = inc_pos;

    inc_pos += include_tag.length();

    std::string inc_template = extract_identifier_word(templatized, inc_pos);
    bool opening_parentheses = check_opening_parenthesis(templatized, inc_pos);
    size_t i = inc_pos;
    bool closing_comment_found = false;

    while(i < templatized.length() && ! closing_comment_found)
    {
        std::string var_name = extract_identifier_word(templatized, i, {':','='});
        if(i == templatized.length() || var_name.empty())
        {
            if(templatized[i] == '#' && !closing_comment_found) // This should be the closing comment
            {
                check_closing_comment(templatized, i, include_tag_end_position);
            }

            break;
        }

        // now read the variable value
        std::string var_value = "";
        bool var_value_read = false;
        while(!var_value_read && i < templatized.length())
        {
            if(templatized[i] == '"') // string starts. Do not read the '"'
            {
                i++; // skip the '"'

                bool string_read = false;
                while(i < templatized.length() && !string_read)
                {
                    if(templatized[i] == '\\') // Skip the backslash from the escape sequence
                    {
                        i++;
                    }

                    var_value += templatized[i++];

                    if(i == templatized.length())
                    {
                        break;
                    }

                    if(templatized[i] == '"' && templatized[i-1] != '\\')
                    {
                        string_read = true;
                    }
                }

                if(i == templatized.length()) // invalid syntax
                {
                    break;
                }

                i++; // skip closing quote
            }
            else
            {
                // see if this is a closing parenthesis, but only if there was an opening one
                if(opening_parentheses && templatized[i] == ')')
                {
                    var_value_read = true;

                    // now see if there is a closing comment. Everything after it will be ignored
                    while(i < templatized.length() && !closing_comment_found)
                    {
                        i ++;
                        skip_whitespace(templatized, i);
                        if(i < templatized.length() && templatized[i] == '#') // This should be the closing comment
                        {
                            closing_comment_found = check_closing_comment(templatized, i, include_tag_end_position);
                        }
                    }
                }

                if(templatized[i] == '#' && !closing_comment_found) // This should be the closing comment
                {
                    closing_comment_found = var_value_read = check_closing_comment(templatized, i, include_tag_end_position);
                }

                if(templatized[i] == '{' && i<templatized.length() && templatized[i + 1] == '#') // The substitution of a variable with another one from this level
                {
                    // Read in the entire variable and simply change the var_value to the one in our objects' map
                    i+=2; // skip {#
                    std::string upperlevel_var_name = extract_identifier_word(templatized, i, {'}'});

                    if(i == templatized.length() || upperlevel_var_name.empty())
                    {
                        break;
                    }

                    if(kps.count(upperlevel_var_name) != 0)
                    {
                        var_value += kps[upperlevel_var_name];
                    }
                }

                // and comma (or space) separatin the variables?
                if(i < templatized.length() &&  (templatized[i] == ',' || iswspace(templatized[i])) )
                {
                    var_value_read = true;
                    i++;
                    skip_whitespace(templatized, i);
                }

                // still not read the value?
                if(i < templatized.length() && !var_value_read)
                {
                    var_value += templatized[i++];
                }
            }
        }

        skip_whitespace(templatized, i);

        // now insert into local_pairs the keys and values
        local_pairs.insert(make_pair(var_name, var_value));
    }

    // and now read in the template from inc_template
    // and replace everything that is supposed to be replaced
    std::string inc_content = template_warehouse::instance().getTemplateContent( inc_template );
    std::string inc_templatized = stringholder(inc_content).templatize(local_pairs).get();

    templatized.erase(include_tag_start_position, include_tag_end_position - include_tag_start_position);
    templatized.insert(include_tag_start_position, inc_templatized);
    return templatized;
}

std::string templater_base::resolve_structure_declaration(size_t struct_pos, std::string templatized)
{
    std::size_t include_tag_start_position = struct_pos;

    struct_pos += struct_tag.length();

    std::string struct_name = extract_identifier_word(templatized, struct_pos);
    bool opening_parentheses = check_opening_parenthesis(templatized, struct_pos);

    add_structure_decl(struct_name, struct_name);

    // and now read in the structure members' names
    size_t i = struct_pos;
    bool closing_comment_found = false;
    std::size_t include_tag_end_position = std::string::npos;

    while (i < templatized.length() && !closing_comment_found)
    {
        std::string member_name = "";
        while(i < templatized.length() && (isalnum(templatized[i]) || templatized[i] == '_'))
        {
            member_name += templatized[i++];
        }
        if(member_name.empty()) // syntax error, just give back what came out
        {
            return templatized;
        }

        skip_whitespace(templatized, i);

        // var_name now is the name of the structure's member, make it
        // a default value
        get_structure(struct_name)[member_name] = "";

        // now this should point either to a space or a ","
        i++;

        // se if we have a "," coming up
        if(templatized[i] == ',')
        {
            i ++;
            skip_whitespace(templatized, i);
        }

        // see if this is a closing parenthesis, but only if there was an opening one
        if(opening_parentheses && templatized[i] == ')')
        {

            // now see if there is a closing comment. Everything after it will be ignored
            while(i < templatized.length() && !closing_comment_found)
            {
                i ++;
                skip_whitespace(templatized, i);
                if(i < templatized.length() && templatized[i] == '#') // This should be the closing comment
                {
                    closing_comment_found = check_closing_comment(templatized, i, include_tag_end_position);
                }
            }
        }

        if(templatized[i] == '#' && !closing_comment_found) // This should be the closing comment
        {
            closing_comment_found = check_closing_comment(templatized, i, include_tag_end_position);
        }
    }

    templatized.erase(include_tag_start_position, include_tag_end_position - include_tag_start_position);
    return templatized;
}

templater_base &templater_base::templatize(const template_struct &s)
{
    precalculated = "";
    std::string templatized = get(); // will initialize the parameters
    if(!m_parameters.empty())
    {
        for(auto p : m_parameters)
        {
            if(p.first == s.name)
            {
                for(auto x : s.struct_members)
                {
                    std::string fullname = s.name + "." + x.first;
                    kps.insert(make_pair(fullname, unafrog::utils::to_string(x.second)));
                }
            }
        }
    }
    templatized = stringholder(templatized).templatize(kps).get();
    templatized = resolve_ifeqs(templatized);
    templatized = resolve_scripts(templatized);
    precalculated = templatized;

    return *this;
}

std::string templater_base::resolve_loops(std::string templatized, const template_vector_par &v)
{
    size_t loop_pos = templatized.find(loop_tag);
    while(loop_pos != std::string::npos)
    {
        // find the stuff between the iterate and end iterate tags
        std::size_t loop_tag_end_position = std::string::npos;
        std::size_t loop_tag_start_position = loop_pos;

        loop_pos += loop_tag.length();

        // see on what are we iterating through
        std::string loop_target = extract_identifier_word(templatized, loop_pos);

        // there should be nothing more after this
        if(!check_closing_comment(templatized, loop_pos, loop_tag_end_position))
        {
            return templatized;
        }

        // now find the end iterate tag
        size_t end_loop_pos = templatized.find(endloop_tag, loop_tag_end_position);
        size_t save_endpos = end_loop_pos;
        end_loop_pos += endloop_tag.length();
        size_t endloop_endpos = std::string::npos;

        size_t temp = end_loop_pos;

        while(temp != std::string::npos)
        {
            size_t unused = temp;

            std::string endloop_target = extract_identifier_word(templatized, unused);
            if(endloop_target == loop_target)
            {
                if(!check_closing_comment(templatized, unused, endloop_endpos))
                {
                    return templatized;
                }

                break;
            }

            temp = templatized.find(endloop_tag, temp);
            save_endpos = temp;
            temp += endloop_tag.length();
        }

        // now between loop_tag_end_position and save_endpos there is the strign we need
        std::string loop_content = templatized.substr(loop_tag_end_position, save_endpos - loop_tag_end_position);

        std::string final_loop_content = "";

        // and now insert a bunch of kps for the elements in v
        if(!m_parameters.empty())
        {
            size_t c = 0;
            for(auto ts : v.value())
            {
                c++;
                for(const auto& p : m_parameters)
                {
                    if(p.first == v.name())
                    {
                        stringholder sh(loop_content);

                        for(auto x : ts.struct_members)
                        {
                            //if(x.first == loop_target)
                            {
                                std::string fullname = v.name() + "." + x.first;
                                std::string fullname_to_replace_with = v.name() + "." + x.first + ":" + std::to_string(c);

                                kps.insert(make_pair(fullname_to_replace_with, unafrog::utils::to_string(x.second)));
                                sh.replaceAll("{#" + fullname + "}", "{#" + fullname_to_replace_with + "}");
                            }
                        }

                        final_loop_content += sh.get();
                    }
                }
            }
        }
        // now fetch out the substring (loop_tag_end_position, save_endpos - loop_tag_end_position) from templatized
        // and replace with final_loop_content

        if(v.name() == loop_target)
        {
            std::string part1 = templatized.substr(0, loop_tag_end_position);

            part1.erase(loop_tag_start_position, loop_tag_end_position - loop_tag_start_position);
            part1 += final_loop_content;
            part1 += templatized.substr(endloop_endpos);

            templatized = part1;
            loop_pos = templatized.find(loop_tag);
        }
        else
        {
            loop_pos = templatized.find(loop_tag, endloop_endpos);
        }
    }

    templatized = stringholder(templatized).templatize(kps).get();
    precalculated = templatized;
    return templatized;
}

std::string templater_base::resolve_ifeqs(std::string templatized)
{
    bool done = false;
    while(!done)
    {
        size_t ifeq_pos = templatized.find(ifeq_tag);
        if(ifeq_pos != std::string::npos)
        {
            while(ifeq_pos != std::string::npos)
            {
                templatized = resolve_ifeq(ifeq_pos, templatized);
                ifeq_pos = templatized.find(ifeq_tag);
            }
        }
        else
        {
            done = true;
        }
    }
    return templatized;
}

templater_base &templater_base::templatize(const template_vector_par &v)
{
    do_not_resolve_in_get();
    std::string templatized = precalculated.empty() ? get() : precalculated;
    do_resolve_in_get();

    templatized = resolve_loops(templatized, v);
    templatized = resolve_ifeqs(templatized);
    templatized = resolve_scripts(templatized);
    precalculated = templatized;

    return *this;
}

void templater_base::skip_whitespace(const std::string& templatized, size_t& i)
{
    while(i < templatized.length() && std::iswspace(templatized[i]))
    {
        i++;
    }
}

std::string templater_base::resolve_parameters(size_t parameters_pos, std::string templatized)
{
    std::size_t include_tag_end_position = std::string::npos;
    std::size_t include_tag_start_position = parameters_pos;

    parameters_pos += in_tag.length();
    skip_whitespace(templatized, parameters_pos);

    size_t i = parameters_pos;
    bool closing_comment_found = false;
    while(i < templatized.length() && ! closing_comment_found)
    {
        // first: read in the variable name
        char c_sep = 32;
        std::string var_name = extract_identifier_word(templatized, i, {':', ' '}, {}, std::move(c_sep));

        if(i == templatized.length() || var_name.empty())
        {
            return templatized;
        }

        std::string var_type = "";
        bool iterable = false;

        if(c_sep == ':')
        {
            // now read the variable type
            var_type = extract_identifier_word(templatized, i);

            if(i < templatized.length() && templatized[i] == '[')
            {
                i ++;
                skip_whitespace(templatized, i);
                if(i < templatized.length() &&  templatized[i] == ']')
                {
                    i ++;
                }
                iterable = true;
            }
        }

        add_parameter(var_name, var_type, iterable);

        skip_whitespace(templatized, i);

        // se if we have a "," coming up
        if(templatized[i] == ',')
        {
            i ++;
            skip_whitespace(templatized, i);
        }

        if(templatized[i] == '#') // This should be the closing comment
        {
            closing_comment_found = check_closing_comment(templatized, i, include_tag_end_position);
        }
    }

    templatized.erase(include_tag_start_position, include_tag_end_position - include_tag_start_position);
    return templatized;
}

std::string templater_base::resolve_ifs(size_t if_pos, std::string templatized)
{
    size_t i = if_pos;

    // skip the <!--#if
    i += if_tag.length();

    // and any whitespace that might follow
    skip_whitespace(templatized, i);

    // now extract the variable
    if(i < templatized.length())
    {
        char c_sep = 32;
        std::string var_name = extract_identifier_word(templatized, i, {' ', '#'}, {}, std::move(c_sep));
        skip_whitespace(templatized, i);

        std::string var_value = "";
        // now if var_name is empty remove the if and it's body.
        if(kps.count(var_name))
        {
            var_value = kps[var_name];
        }

        std::string closing_endif_tag = endif_tag + " " + var_name;
        std::string closing_else_tag = else_tag + " " + var_name;

        if(var_value.empty())
        {
            // search for the first <!--#endif var_name
            size_t endif_pos = templatized.find(closing_endif_tag);
            if(endif_pos != std::string::npos)
            {
                // search for the first <!--#else before the endif tag

                size_t else_pos = templatized.find(closing_else_tag);
                if(else_pos < endif_pos)
                {
                    // this is the else of this if, removing the content of the if till the else
                    templatized = templatized.substr(0, if_pos) + templatized.substr(else_pos + closing_else_tag.length() + 4);
                    // we still need to remove the endif
                    size_t final_endif_pos = templatized.find(closing_endif_tag);
                    if(final_endif_pos != std::string::npos)
                    {
                        templatized = templatized.substr(0, final_endif_pos) + templatized.substr(final_endif_pos + closing_endif_tag.length() + 4);
                    }

                }
                else
                {
                    // no else, just remove the if tag
                    templatized = templatized.substr(0, if_pos) + templatized.substr(endif_pos + closing_endif_tag.length() + 4);
                }
            }
            else
            {
                // oops, user forgot the enclosing endif, just return what is here
                return templatized;
            }
        }
        else // remove the comments :)
        {
            size_t endif_pos = templatized.find(closing_endif_tag);

            if(endif_pos != std::string::npos)
            {
                size_t remove_start_pos = endif_pos;
                std::string remove_tag = closing_endif_tag;
                size_t additional_remove = 0; // count ofchars between else and endif
                // search for the first <!--#else before the endif tag
                size_t else_pos = templatized.find(closing_else_tag);
                if(else_pos < endif_pos)
                {
                    remove_start_pos = else_pos;
                    additional_remove = endif_pos - remove_start_pos;
                }

                std::string before_if = templatized.substr(0, if_pos);
                std::string between = templatized.substr(i + 3, remove_start_pos - i - 3);
                std::string after_endif = templatized.substr(remove_start_pos + additional_remove + closing_endif_tag.length() + 4);

                templatized =  before_if + between + after_endif;
            }
            else
            {
                // oops, user forgot the enclosing endif, just return what is here
                return templatized;
            }
        }
    }

    return templatized;
}

std::string templater_base::resolve_ifeq(size_t if_pos, std::string templatized)
{
    size_t i = if_pos;

    // skip the <!--#ifeq
    i += ifeq_tag.length();

    // and any whitespace that might follow
    skip_whitespace(templatized, i);

    // now extract the variable
    if(i < templatized.length())
    {
        char c_sep = 32;
        std::string what_to = extract_identifier_word(templatized, i, {' ', '#'}, {',','-'}, std::move(c_sep));
        skip_whitespace(templatized, i);

        size_t comma_pos = what_to.find(',');
        bool are_eq = true;
        if(comma_pos == std::string::npos)
        {
            are_eq = false;
        }
        else
        {
            std::string before = what_to.substr(0, comma_pos);
            std::string after =  what_to.substr(comma_pos + 1);

            are_eq = before == after;
        }


        if(!are_eq)
        {
            // search for the first <!--#endifeq
            size_t endif_pos = templatized.find(endifeq_tag);
            if(endif_pos != std::string::npos)
            {
                templatized = templatized.substr(0, if_pos) + templatized.substr(endif_pos + endifeq_tag.length() + 4);
            }
        }
        else // remove the comments :)
        {
            size_t endif_pos = templatized.find(endifeq_tag);

            if(endif_pos != std::string::npos)
            {
                size_t remove_start_pos = endif_pos;
                std::string before_if = templatized.substr(0, if_pos);
                std::string between = templatized.substr(i + 3, remove_start_pos - i - 3);
                std::string after_endif = templatized.substr(remove_start_pos + endifeq_tag.length() + 4);

                templatized =  before_if + between + after_endif;
            }

        }
    }

    return templatized;
}

std::string templater_base::resolve_defines(std::string content)
{
    size_t define_pos = content.find(define_tag);

    std::vector<std::pair<size_t, size_t>> strings_to_remove; // we will remove the define tags from in here
    if(define_pos != std::string::npos)
    {
        while(define_pos != std::string::npos)
        {
            bool define_read = false;
            // skipping the tag
            size_t i = define_pos + define_tag.length();
            // skipping the whitespace after the tag
            skip_whitespace(content, i);
            while(!define_read)
            {

                std::string var_name = "";
                std::string var_value = "";
                // now comes a list of comma separated variable=value assignments or just simply variable
                while(i < content.length() && content[i] != '=' && content[i] != ',' && content[i] != '#'
                      && (isalnum(content[i]) || content[i] == '_' ))
                {
                    var_name += content[i];
                    i ++;
                }
                // now if content[i] == '=' we need to parse out the value from it
                if(content[i] == '=')
                {
                    i ++;
                    bool string_comes = false;
                    if(content[i] == '"') // string comes after this?
                    {
                        i++;
                        string_comes = true;
                    }
                    while( (i < content.length() && content[i] != ',' && content[i] != '#' && !string_comes)
                           || (i < content.length() && string_comes && content[i] != '"'))
                    {
                        if(content[i] != '\\') // escaping a quote in the string
                        {
                            var_value += content[i];
                        }
                        else
                        {
                            i++;
                            var_value += content[i];
                        }
                        i++;
                    }
                }
                else
                {
                    var_value = "true";
                }
                kps.insert({var_name, var_value});
                if(content[i] == '#')
                {
                    i++;
                    if(i +2 < content.length() && content[i] == '-' && content[i+1] == '-' && content[i+2]=='>')
                    {
                        i += 3; // skip the closing comment
                        define_read = true;
                    }
                    else
                    {
                        // this is a malformed entry, just give up
                        return content;
                    }
                }
                else
                {
                    if(content[i] == ',')
                    {
                        i++;
                    }
                }
            }

            strings_to_remove.insert(strings_to_remove.begin(), {define_pos, i});
            define_pos = content.find(define_tag, i);
        }
    }
    // now walk through the strings_to_remove and delete from the content everything from there
    for(const auto& beg_end_pos : strings_to_remove)
    {
        content.erase(beg_end_pos.first, beg_end_pos.second - beg_end_pos.first);
    }

    return content;
}

std::string templater_base::resolve_scripts(std::string templatized)
{
    bool done = false;
    while(!done)
    {
        size_t script_tag_pos = templatized.find(script_tag);
        if(script_tag_pos != std::string::npos)
        {
            while(script_tag_pos != std::string::npos)
            {
                templatized = resolve_script(script_tag_pos, templatized);
                script_tag_pos = templatized.find(script_tag);
            }
        }
        else
        {
            done = true;
        }
    }
    return templatized;
}

std::string templater_base::resolve_script(size_t pos, const std::string& content)
{
    size_t endscript_start_pos = content.find(endscript_tag);
    size_t old_pos = pos;

    // skip the tag
    pos += script_tag.length();
    // skip the space(s) followwing the "script"
    while(isspace(content[pos])) pos += 1;

    std::string script_type = "";
    while(content[pos] != '#' && pos < content.length())
    {
        script_type += content[pos++];
    }

    if(pos == content.length())
    {
        set_error("Script tag at", old_pos, "is not closed");
        return content.substr(0, old_pos);
    }

    static const auto script_types = {"python"};

    bool found = false;
    for(const auto& st : script_types)
    {
        if(st == script_type) found = true;
    }

    if(!found)
    {
        set_error("Unsupported scripting language", script_type);
        return content.substr(0, old_pos);
    }

    // pos points to the script closing tag
    std::string before_script = content.substr(0, old_pos);
    std::string between = std::string(content.begin() + pos + 4,  // #--> is the 4
                                      content.begin() + endscript_start_pos);
    std::string after_script = content.substr(endscript_start_pos + endscript_tag.length() + 4);

    std::cerr << "Trying to run:" << std::endl<< between << std::endl;

    // run the code in <<between>> depending on the requested interpreter

    PyImport_AppendInittab("emb", emb::PyInit_emb);
    Py_Initialize();
    PyImport_ImportModule("emb");

    // here comes the ***magic***
    std::string buffer;
    {
        // switch sys.stdout to custom handler
        emb::stdout_write_type write = [&buffer] (std::string s) { buffer += s; };
        emb::set_stdout(write);
        PyRun_SimpleString(between.c_str());
        emb::reset_stdout();
    }

    Py_Finalize();


    return before_script + unafrog::utils::trim(buffer) + after_script;
}

void stringholder::replaceAll(const std::string &from, const std::string &to)
{
    if(from.empty())
    {
        return;
    }
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos)
    {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}

stringholder &stringholder::templatize(const std::map<std::string, std::string> &m)
{
    for(auto it = m.begin(); it != m.end(); ++ it)
    {
        replaceAll(std::string("{#") + it->first + std::string("}"), it->second );
    }
    return *this;
}

template_warehouse &template_warehouse::instance()
{
    static template_warehouse twh;
    return twh;
}

bool template_warehouse::checkTemplates()
{
    for(auto it = templates.begin(); it != templates.end(); ++it)
    {
        if(!it->second->check())
        {
            return false;
        }
    }
    debug() << "All templates OK";
    return true;
}

std::string template_warehouse::getTemplateContent(const std::string &templateName)
{
    if(templates.count(templateName))
    {
        std::shared_ptr<template_base> tb = templates[templateName];
        std::string c = tb->content();
        return c;
    }
    else
    {
        return "Not found:" + templateName;
    }
}

bool template_warehouse::registerTemplate(const std::string &name, template_base *templateClass)
{
    // although this code seems stupid, it is due to the fact that every inclusion of the
    // templater.h will cause a new object to be created, but we want only one
    if(templates.count(name))
    {
        debug() << "Freeing existing template class:" << name;
        delete templateClass;
        return false;
    }
    debug() << "Registering template class:" << name;
    templates[name] = std::shared_ptr<template_base>(templateClass);
    return true;
}


std::string file_template::content() const
{
    std::string fn = TEMPLATES_DIRECTORY + fileName();
    std::ifstream ifs(fn);
    std::string s( (std::istreambuf_iterator<char>(ifs) ),
                   (std::istreambuf_iterator<char>()) );
    return s;
}

bool file_template::check() const
{
    struct stat buffer;
    bool res = (stat ( (TEMPLATES_DIRECTORY +  fileName()).c_str(), &buffer) == 0);
    if(!res)
    {
        debug() << "Check for "<< fileName() << " failed";
    }
    return res;
}
