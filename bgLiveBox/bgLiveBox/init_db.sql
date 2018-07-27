DROP TABLE IF EXISTS Logs;
CREATE TABLE Logs(
id INTEGER PRIMARY KEY NOT NULL AUTO_INCREMENT,
datetime VARCHAR(32) NOT NULL,
ipaddress VARCHAR(16) NOT NULL,
computer_name VARCHAR(32) NOT NULL,
op_type INTEGER NOT NULL,
sub VARCHAR(128) NOT NULL,
obj VARCHAR(128) NOT NULL,
imgurl VARCHAR(256)
);

INSERT
INTO `logs`(datetime, ipaddress, computer_name, op_type, sub, obj, imgurl)
VALUES('2018-07-27_15-20-59', '10.10.1.10', 'AVX', 1, 'ScreenCapture', '', 'http://123.png');

INSERT
INTO `logs`(datetime, ipaddress, computer_name, op_type, sub, obj, imgurl)
VALUES('2018-07-27_15-22-19', '10.10.1.10', 'AVX', 2, 'GetUrl', 'xiaojiejie', 'http://123.png');