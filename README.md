# BioAlert

## Estrutura codigo
- Backend -> Todo o codigo de webservice e base de dados
- Frontend -> Todo o codigo de interface web
- Sensor -> Todo o codigo do ESP32

## Database
jdbc:postgresql://vsgate-s1.dei.isep.ipp.pt:10488/
nome: postgres
password: Pa8Mr9D6zzAl


## Backend

### Como instalar
1. Instalar python 3.11
2. Executar comando "cd backend"
3. Instalar ambiente virtual através do comando: "python -venv ."
4. Activar ambiente virtual através do comando: "Scripts\activate"
    4.1. Deve aparecer na consola (backend)
5. Instalar pacotes atraves do comando: "pip install -r requirements.txt"
6. Entrar na pasta do projeto : "cd bioalert"
7. Criar base de dados: "python manage.py makemigrations"
8. Ativar base de dados: "python manage.py migrate"
9. Criar utilizador: "python manage.py createsuperuser"
10. Ligar servidor: "python manage.py runserver"

### Como executar depois de instalado
1. Executar comando "cd backend"
2. Activar ambiente virtual através do comando: "Scripts\activate"
    2.1. Deve aparecer na consola (backend)
3. Entrar na pasta do projeto : "cd bioalert"
4. Ligar servidor: "python manage.py runserver"