# zipsfx-cpp
zipsfx-cpp - это head stub для zip sfx файла. т.е. программа является "загрузчиком" инсталлятора.

Описание
========

Программа является "загрузчиком" инсталлятора.

Умеет распаковывать во временную папку (переменная окружения TEMP) содержимое архива и запускаnm exe файл из распакованного архива с передачей ему аргументов.

Не требует дополнительных DLL файлов, 
кроме тех что поставляются вместе с Windows (KERNEL32.DLL и MSVCRT.DLL).

Использование
=============

Создание архива
---------------

Упакуйте файлы в zip архив проходящим для вас архиватором. 
Если необходимо включите в архив exe файл, который будет запущен по окончании распаковки архива.

    c:\Documents and Settings\user\Рабочий стол\sample-zip\data>7z a -tzip ..\data.zip *

    7-Zip 4.65  Copyright (c) 1999-2009 Igor Pavlov  2009-02-03
    Scanning

    Creating archive ..\data.zip

    Compressing  index.html
    Compressing  printargs.exe

    Everything is Ok
    
**Ограничение:**

  * Файл не должен превышать 2Гб.
  * Не должен содержать в именах файлов русских символов.

Подготовка параметров распаковки
--------------------------------

Данный шаг не является обязательным, если вам нет нужды специально указывать дополнительные параметры, вы можете пропустить этот шаг.

Создайте текстовый файл [options.txt](https://github.com/gochaorg/zipsfx-cpp/raw/v0.4/dist/Release/MinGW2-Windows/options.txt) со следующим содержанием

    OPTIONS BEGIN {
    # Hello
    hello = SFX ZIP v 0.1 by nt.gocha@gmail.com Sample

    # Delete files on exit
    deleteFiles = true

    # Run after extract
    run = $dir\printargs.exe "$dir"
    } OPTIONS END

### Описание формата файла

  * Файл должен начинаться текстом **`OPTIONS BEGIN {`**
  * Файл должен заканчиваться текстом **`} OPTIONS END`**
  * После *`} OPTIONS END`* не должны следовать какие либо символы , в том числе перевода строк.
  * Начальный и конечный текст ( **`OPTIONS BEGIN {`** и **`} OPTIONS END`** ) - чувствителен к регистру.
  * Файл должен быть в 8-битной кодировке
  * Файл должен быть не большого размера, не более 256Кб.
  * Между **`OPTIONS BEGIN {`** и **`} OPTIONS END`** построчно идут пары значений: **ключ** = **значение**, так же допускается использовать комментарии, предварив их символом решетки - **#**
  * Начальные и конечные пробельные символы в названии ключа и значения не используются, т.е. если вы напишете: <br> `__key_1__`, где `_` - пробелы, то это будет восприниматься программой словно вы написали `key_1`, это же правило применимо к значению.
  * Название ключа чувствительно к регистру
  
### Параметры

  * **`hello`** - Указывает текст выводимый в качестве приветствия. Параметр не является обязательным.
  * **`deleteFiles`** - Указывает что по завершении работы необходимо удалить распакованные файлы.
  * **`run`** - Указывает шаблон запускаемого файла после распаковки. <br/>В шаблон передаются следующие переменные `$dir`, `$args`, `$zip`. 
    * **`$dir`** - Эта переменная указывает на папку, в которую был распакован архив. Везде где упоминается эта переменная в шаблоне будет заменена на ее значение.
    * **`$args`** - аргументы переданные через командную строку - соответствует аргументу `%*` .
    * **`$zip`** - путь к самому zip архиву.
   * **`workDir`** - Указывает рабочий каталог (шаблон) на время исполнения команды *`run`*
   * **`return`** - Значение **`true`** указывает возвращать код завершения команды *`run`*; **`false`** (по умолчанию) - Использовать свое значение
  * **`quoteArgs`** - брать в кавычки аргументы, возможные значения:
    * **`auto`** (по умолчанию) - автоматически, при обнаружении пробелов в аргументе
    * **`always`** - всегда брать в кавычки
    * **`never`** - никогда не брать в кавычки
  * **`showHello`** - Указывает отображать приветствие (**`true`**) или нет (**`false`**)
  * **`showLog`** - Указывает отображать лог действий (**`true`**) или нет (**`false`**)
  * **`useTemp`** - Значение **`true`** (по умолч.) указывает использовать временный каталог для распаковки; **`false`** - будет использован текущий каталог.
  
## Создание EXE файла

Теперь необходимо последовательно "склеить" три файла: загрузчик (**[zipsfx](https://github.com/gochaorg/zipsfx-cpp/raw/v0.4/dist/Release/MinGW2-Windows/zipsfx)**), файл с опциями (**[options.txt](https://github.com/gochaorg/zipsfx-cpp/raw/v0.4/dist/Release/MinGW2-Windows/options.txt)**) и ZIP архив (**data.zip**).

Ниже пример как сделать это в командной строке WINDOWS:

    C:\Documents and Settings\user\Рабочий стол\sample-zip>copy /b zipsfx + options.txt + data.zip install.exe
    zipsfx
    options.txt
    data.zip
    Скопировано файлов:         1.

## Пример выполнения

В командной строке WINDOWS запустите получившийся файл install.exe

    SFX ZIP v 0.1 by nt.gocha@gmail.com Sample
    unpack zip to C:\DOCUME~1\user\LOCALS~1\Temp\temp.zip.content
    extract content
    count entries = 2
    1. index.html
    2. printargs.exe
    print args
    C:\DOCUME~1\user\LOCALS~1\Temp\temp.zip.content\printargs.exe
    C:\DOCUME~1\user\LOCALS~1\Temp\temp.zip.content
    press ENTER/RETURN for exit

    removing C:\DOCUME~1\user\LOCALS~1\Temp\temp.zip
    removing C:\DOCUME~1\user\LOCALS~1\Temp\temp.zip.content

Пояснения:

 * Текст `SFX ZIP v 0.1 by nt.gocha@gmail.com Sample` - этот текст был указа в опции *`hello`*
 * Текст `print args ... press ENTER/RETURN for exit` - отображается программой printargs.exe

Дистрибутив
===========

Актуальная версия **0.4**

Последний актуальный дистрибутив находится по ссылке
https://github.com/gochaorg/zipsfx-cpp/tree/v0.4/dist/Release/MinGW2-Windows

Исходный код https://github.com/gochaorg/zipsfx-cpp/archive/v0.4.tar.gz

В дистирибутив входит

 * [zipsfx](https://github.com/gochaorg/zipsfx-cpp/raw/v0.4/dist/Release/MinGW2-Windows/zipsfx) - бинарный файл, который и представляет из себя "инсталлятор" / sfx stub
 * [options.txt](https://github.com/gochaorg/zipsfx-cpp/raw/v0.4/dist/Release/MinGW2-Windows/options.txt) - файл опций (параметров) инсталятора
