DROP TABLE IF EXISTS bgKeep;
CREATE TABLE bgKeep(
id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
app_name VARCHAR(128) NOT NULL,
room_name VARCHAR(128) NOT NULL
);

INSERT
INTO `bgKeep`(app_name, room_name)
VALUES('�ȱ���', '���ٴ�����');

INSERT
INTO `bgKeep`(app_name, room_name)
VALUES('��С��', '���ٴ�����0');