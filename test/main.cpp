#include <oowe/oowe.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>

template<class Clock, class Duration = typename Clock::duration>
std::ostream & operator << (std::ostream & os, std::chrono::time_point<Clock, Duration> point)
{
    std::time_t      timePoint = std::chrono::system_clock::to_time_t(point);
    struct std::tm * tmPoint = std::localtime(&timePoint);

    os << std::put_time(tmPoint, "%c");

    return os;
}

void httpPostFormNew(oowe::HttpPost & post, oowe::HttpSession & session)
{
    char           namebuffer[] = "name buffer";
    long           namelength = std::strlen(namebuffer);
    char           buffer[] = "test buffer";
    char           htmlbuffer[] = "<HTML>test buffer</HTML>";
    long           htmlbufferlength = std::strlen(htmlbuffer);
    char file1[] = "my-face.jpg";
    char file2[] = "your-face.jpg";
    /* add null character into htmlbuffer, to demonstrate that
       transfers of buffers containing null characters actually work
       */
    htmlbuffer[8] = '\0';

    /* Add simple name/content section */
    post.addNameCopy   ("name")
        .addContentCopy("content")
        .end();

    /* Add simple name/content/contenttype section */
    post.addNameCopy   ("htmlcode")
        .addContentCopy("<HTML></HTML>")
        .addContentType("text/html")
        .end();

    /* Add name/ptrcontent section */
    post.addNameCopy("name_for_ptrcontent")
        .addContent (buffer)
        .end();

    /* Add ptrname/ptrcontent section */
    post.addName   (namebuffer, namelength)
        .addContent(buffer)
        .end();

    /* Add name/ptrcontent/contenttype section */
    post.addNameCopy   ("html_code_with_hole")
        .addContent    (htmlbuffer, htmlbufferlength)
        .addContentType("text/html")
        .end();

    /* Add simple file section */
    post.addNameCopy("markdown")
        .addFile    ("README.md")
        .end();

    /* Add file/contenttype section */
//    post.addNameCopy   ("picture")
//        .addFile       ("my-face.jpg")
//        .addContentType("image/jpeg")
//        .end();

    /* Add two file section */
//    post.addNameCopy("pictures")
//        .addFile    ("my-face.jpg")
//        .addFile    ("your-face.jpg")
//        .end();

    /* Add a buffer to upload */
//    post.add(CURLFORM_COPYNAME, "name", CURLFORM_BUFFER, "data", CURLFORM_BUFFERPTR, record, CURLFORM_BUFFERLENGTH, record_length, CURLFORM_END);
//    post.addNameCopy      ("name")
//        .addBufferFileName("data")
//        .addBuffer        (record, record_length)
//        .end();

    /* Add the content of a file as a normal post text value */
//    post.add(CURLFORM_COPYNAME, "filecontent", CURLFORM_FILECONTENT, ".bashrc", CURLFORM_END);
    post.addNameCopy("filecontent")
        .addFileContent("/user4/c_oegpp/t0048326/.bashrc")
        .end();

    /* Add two file section using CURLFORM_ARRAY */

    /* no option needed for the end marker */
//   post.addNameCopy("pictures")
//        .addFile(file1)
//        .addFile(file2)
//        .end();

    /* Set the form info */
    session.setHttpPost(post);
}

void httpPostForm(oowe::HttpPost & post, oowe::HttpSession & session)
{
    char           namebuffer[] = "name buffer";
    long           namelength = std::strlen(namebuffer);
    char           buffer[] = "test buffer";
    char           htmlbuffer[] = "<HTML>test buffer</HTML>";
    long           htmlbufferlength = std::strlen(htmlbuffer);
//    struct curl_forms     forms[3];
    char file1[] = "my-face.jpg";
    char file2[] = "your-face.jpg";
    /* add null character into htmlbuffer, to demonstrate that
       transfers of buffers containing null characters actually work
       */
    htmlbuffer[8] = '\0';

    /* Add simple name/content section */
    post.add(CURLFORM_COPYNAME, "name", CURLFORM_COPYCONTENTS, "content", CURLFORM_END);

    /* Add simple name/content/contenttype section */
    post.add(CURLFORM_COPYNAME, "htmlcode", CURLFORM_COPYCONTENTS, "<HTML></HTML>", CURLFORM_CONTENTTYPE, "text/html", CURLFORM_END);

    /* Add name/ptrcontent section */
    post.add(CURLFORM_COPYNAME, "name_for_ptrcontent", CURLFORM_PTRCONTENTS, buffer, CURLFORM_END);

    /* Add ptrname/ptrcontent section */
    post.add(CURLFORM_PTRNAME, namebuffer, CURLFORM_PTRCONTENTS, buffer, CURLFORM_NAMELENGTH, namelength, CURLFORM_END);

    /* Add name/ptrcontent/contenttype section */
    post.add(CURLFORM_COPYNAME, "html_code_with_hole", CURLFORM_PTRCONTENTS, htmlbuffer, CURLFORM_CONTENTSLENGTH, htmlbufferlength, CURLFORM_CONTENTTYPE, "text/html", CURLFORM_END);

    /* Add simple file section */
    post.add(CURLFORM_COPYNAME, "picture", CURLFORM_FILE, "my-face.jpg", CURLFORM_END);

    /* Add file/contenttype section */
    post.add(CURLFORM_COPYNAME, "picture", CURLFORM_FILE, "my-face.jpg", CURLFORM_CONTENTTYPE, "image/jpeg", CURLFORM_END);

    /* Add two file section */
    post.add(CURLFORM_COPYNAME, "pictures", CURLFORM_FILE, "my-face.jpg", CURLFORM_FILE, "your-face.jpg", CURLFORM_END);

    /* Add a buffer to upload */
//    post.add(CURLFORM_COPYNAME, "name", CURLFORM_BUFFER, "data", CURLFORM_BUFFERPTR, record, CURLFORM_BUFFERLENGTH, record_length, CURLFORM_END);

    /* Add the content of a file as a normal post text value */
    post.add(CURLFORM_COPYNAME, "filecontent", CURLFORM_FILECONTENT, ".bashrc", CURLFORM_END);

//    /* Add two file section using CURLFORM_ARRAY */
//    forms[0].option = CURLFORM_FILE;
//    forms[0].value  = file1;
//    forms[1].option = CURLFORM_FILE;
//    forms[1].value  = file2;
//    forms[2].option  = CURLFORM_END;

    /* no option needed for the end marker */
//    post.add(CURLFORM_COPYNAME, "pictures", CURLFORM_ARRAY, forms, CURLFORM_END);

    /* Set the form info */
    session.setHttpPost(post);
}

int main(int argc, char ** argv)
{
    oowe::StdOutputStream<std::ofstream> output("/dev/null");
    oowe::StdInputStream <std::ifstream> input ("README.md");
    oowe::HttpSession session;
    oowe::HttpPost    post;

    // Add form
    httpPostForm   (post, session);
    httpPostFormNew(post, session);

    session.setFileTime(true);
    session.setOutputStream(&output);
    session.setInputStream (&input );
    session.setUpload(true);
    session.setFollowLocation(true);

    for(int i=1 ; i<argc; i++)
    {
        session.setUrl(argv[i]);

        std::cout << "***********************************************************************" << std::endl;
        std::cout << "BEGIN Fetching \"" << session.getEffectiveUrl() << '"' << std::endl;
        std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
        session.perform();
        std::chrono::time_point<std::chrono::system_clock> end   = std::chrono::system_clock::now();

        std::cout << "    HTTP response code = " << session.getResponseCode() << std::endl;
        std::cout << "    File time = \"" << session.getFileTime() << '"' << std::endl;


        std::chrono::duration<double> elapsed = end - start; 
        std::cout << "    START " << start << std::endl;
        std::cout << "        |  " << std::endl << std::fixed;
        std::cout << "        |---> NAME LOOKUP                                   = " << session.getNameLookupTime()   .count() << 's' << std::endl;
        std::cout << "        |-----|---> CONNECT                                 = " << session.getConnectTime()      .count() << 's' << std::endl;
        std::cout << "        |-----|-----|---> APP CONNECT                       = " << session.getAppConnectTime()   .count() << 's' << std::endl;
        std::cout << "        |-----|-----|-----|---> PRETRANSFER                 = " << session.getPreTransferTime()  .count() << 's' << std::endl;
        std::cout << "        |-----|-----|-----|-----|---> START TRANSFERT       = " << session.getStartTransferTime().count() << 's' << std::endl;
        std::cout << "        |-----|-----|-----|-----|-----|---> TOTAL           = " << session.getTotalTime()        .count() << 's' << std::endl;
        std::cout << "        |-----|-----|-----|-----|-----|---> REDIRECT TIME   = " << session.getRedirectTime()     .count() << 's' << std::endl;
        std::cout << "    END = " << end << " (" << elapsed.count() << "s)" << std::endl;

        std::cout << "    Cookies = " << session.getCookieList() << std::endl;
        std::cout << "    SSL Engines = " << session.getSslEngines() << std::endl;

        std::cout << "END   Fetching \"" << session.getEffectiveUrl() << '"' << std::endl;
        std::cout << "***********************************************************************" << std::endl << std::endl;
    }

    return 0;
}
