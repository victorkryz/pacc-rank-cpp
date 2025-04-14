#include "downloader.h"
#include "protocol_provider.h"
#include "protocol_processor.h"


const static std::string PROTOCOL_REMOTE_REF = "https://rgk.vote.mod.gov.ua/protocol.txt";
 
int main(int argc, char* argv[])
{
   int result(0);

   try
   {  
      std::shared_ptr<ContentProvider> protocol_provider =
         std::make_shared<ProtocolProvider>(PROTOCOL_REMOTE_REF, std::cout);

      ProtocolProcessor processor(protocol_provider, std::cout);

      processor.process();
      processor.print_report();
   }
   catch(const std::exception& e)
   {
      std::cerr << e.what() << '\n';
      result = 1;
   }
   
   return result;
}	



