# Общие принципы #

  * mail-движок - vmime
  * состоит из UI и мыльной части
  * работает с мылами не напрямую, а через кешер

# Структура данных #

Сами сообщения хранятся локально, данные из сообщений - в базе.
  * UserSetting: Common cfg - всякие умолчания
  * UserData: общая db + кеши мыльниц + настройки каждой мыльницы

База (sqlite):
  * mailbox (id, ? settings)
  * folder (иерархический список) (mailbox\_id, name, parent)
  * message (заголовки) (uid, ...)
  * folder-message (m2m) ?

Файлы: одним плоским списком в папке mailbox.

папка mailbox:
  * Create: upper no +1 or 1
  * Read: nothing
  * Update: nothing
  * Delete: rmdir

# Порядок обновления #

## Download ##

  * скачивать по одному, в thread'е, с индикатором
  * msg выкачивать только по запросу

  1. folders:
    1. get folder list
    1. insert new
    1. remove deleted
    1. recuring
  1. msgs:
    1. get msg count
    1. get uids (tmp table)
    1. download new (headers)
    1. del absent (msgs) ???
    1. load whole msg on demand

## Upload ##

# Test #

  * time: fetchall vs fetchone: fetchall 40% faster.
  * time: fetchall(uid) vs fetchall(all): fetchall(uid) 15 times faster.

# Ideas #