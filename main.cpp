#include <iostream>
#include <exception>
#include <list>
#include <curl/curl.h>
#include <string>
#include <fstream>

//Funciones auxiliares
static size_t myCallback(void* contents, size_t size, size_t nmemb, void* userp);

/*
* Main de prueba.
* Realiza un GET por HTTP a la dirección solicitada.
*
*/

int main(void)
{
	std::string myString;
	CURL* curl;					//Variable donde vamos a guardar las configuraciones de una transferencia
	CURLcode res;
	std::string readString;
	std::cout << "Please enter a path to server" << std::endl;
	std::cin >> myString;
	//Elejimos el path al archivo que queremos recibir en el servidor.
	//std::string path = "http://www.cplusplus.com/reference/string/string/find/";
	std::string path = myString;
	// Primero realizamos la inicialización de cURL.
	curl = curl_easy_init();

	if (curl)
	{
		//Seteamos primero la pagina donde nos vamos a conectar.
		curl_easy_setopt(curl, CURLOPT_URL, path.c_str());

		//Le decimos a cURL que imprima todos los mensajes que se envían entre cliente y servidor.
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

		// Le decimos a cURL que trabaje con HTTP.
		curl_easy_setopt(curl, CURLOPT_PROTOCOLS, CURLPROTO_HTTP);

		//Le decimos a cURL que cuando haya que escribir llame a myCallback
		//y que use al string readString como user data.
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, myCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readString);

		//Realiza el requeste. res tendrá el codigo de error
		//Con lo de abajo le decimos a curl que intente conectarse a la página
		//Recordar que easy_perform es bloqueante, durante este TP vamos a trabajar así
		res = curl_easy_perform(curl);

		// Nos fijamos si hubo algún error
		if (res != CURLE_OK)
		{
			std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
		}

		// Hago un clean up antes de realizar un nuevo query.
		curl_easy_cleanup(curl);
	}
	std::cout << readString.c_str() << std::endl;

	std::ofstream ofs("received.txt", std::ofstream::out);
	ofs << readString.c_str() << std::endl;
	ofs.close();
	
	getchar();
	return 0;
}

//Concatena lo recibido en content a s
static size_t myCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
	size_t realsize = size * nmemb;
	char* data = (char*)contents;
	//fprintf(stdout, "%s",data);
	//std::cout << "Recibi cosas!" << std::endl;
	std::string* s = (std::string*)userp;
	s->append(data, realsize);
	return realsize;						//recordar siempre devolver realsize
}