-- phpMyAdmin SQL Dump
-- version 4.7.0
-- https://www.phpmyadmin.net/
--
-- Host: 127.0.0.1
-- Erstellungszeit: 11. Sep 2017 um 22:10
-- Server-Version: 10.1.25-MariaDB
-- PHP-Version: 7.0.21

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET AUTOCOMMIT = 0;
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Datenbank: `hbluz`
--

-- --------------------------------------------------------

--
-- Tabellenstruktur für Tabelle `programs`
--

CREATE TABLE `programs` (
  `id` int(11) NOT NULL,
  `user_id` int(11) NOT NULL,
  `program_name` tinytext NOT NULL,
  `date_posted` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `is_autosave` tinyint(1) NOT NULL,
  `raw_json` mediumtext NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Daten für Tabelle `programs`
--

INSERT INTO `programs` (`id`, `user_id`, `program_name`, `date_posted`, `is_autosave`, `raw_json`) VALUES
(1, 1, 'Phalaenopsis', '2017-09-07 20:25:19', 0, '{\r\n  \"Program\": {\r\n    \"DeviceName\": \"HBLUZ_1\",\r\n    \"ProgramName\": \"Phalaenopsis\",\r\n    \"SyncTime\": \"20:08:57\",\r\n    \"SyncDate\": \"2017-08-09\",\r\n    \"ProgramDateStart\": \"2018-07-25\",\r\n    \"Lightsettings\": {\r\n      \"Lightsetting\": [\r\n        {\r\n          \"Description\": \"Morgens\",\r\n          \"RelativeStart\": 1,\r\n          \"Duration\": 182,\r\n          \"TimeStart\": \"00:00\",\r\n          \"TimeEnd\": \"00:00\",\r\n          \"Intensity\": \"0\",\r\n          \"Wavelength\": \"0\"\r\n        },\r\n        {\r\n          \"Description\": \"Unbenannt\",\r\n          \"RelativeStart\": 0,\r\n          \"Duration\": 162,\r\n          \"TimeStart\": \"00:00\",\r\n          \"TimeEnd\": \"00:00\",\r\n          \"Intensity\": \"0\",\r\n          \"Wavelength\": \"0\"\r\n        }\r\n      ]\r\n    }\r\n  }\r\n}'),
(2, 2, 'Hanf', '2017-09-07 20:25:19', 0, '{\r\n  \"Program\": {\r\n    \"DeviceName\": \"HBLUZ_1\",\r\n    \"ProgramName\": \"Hanf\",\r\n    \"SyncTime\": \"20:08:57\",\r\n    \"SyncDate\": \"2017-08-09\",\r\n    \"ProgramDateStart\": \"2018-07-25\",\r\n    \"Lightsettings\": {\r\n      \"Lightsetting\": [\r\n        {\r\n          \"Description\": \"Morgens\",\r\n          \"RelativeStart\": 1,\r\n          \"Duration\": 182,\r\n          \"TimeStart\": \"00:00\",\r\n          \"TimeEnd\": \"00:00\",\r\n          \"Intensity\": \"0\",\r\n          \"Wavelength\": \"0\"\r\n        },\r\n        {\r\n          \"Description\": \"Unbenannt\",\r\n          \"RelativeStart\": 0,\r\n          \"Duration\": 162,\r\n          \"TimeStart\": \"00:00\",\r\n          \"TimeEnd\": \"00:00\",\r\n          \"Intensity\": \"0\",\r\n          \"Wavelength\": \"0\"\r\n        }\r\n      ]\r\n    }\r\n  }\r\n}'),
(18, 1, 'Phalaenopsis2', '2017-09-07 21:05:40', 0, '{\n  \"Program\": {\n    \"DeviceName\": \"HBLUZ_1\",\n    \"ProgramName\": \"Phalaenopsis2\",\n    \"SyncTime\": \"20:08:57\",\n    \"SyncDate\": \"2017-08-09\",\n    \"ProgramDateStart\": \"2018-07-25\",\n    \"Lightsettings\": {\n      \"Lightsetting\": [\n        {\n          \"Description\": \"Morgens\",\n          \"RelativeStart\": 1,\n          \"Duration\": 182,\n          \"TimeStart\": \"00:00\",\n          \"TimeEnd\": \"00:00\",\n          \"Intensity\": \"0\",\n          \"Wavelength\": \"0\"\n        },\n        {\n          \"Description\": \"Unbenannt\",\n          \"RelativeStart\": 0,\n          \"Duration\": 162,\n          \"TimeStart\": \"00:00\",\n          \"TimeEnd\": \"00:00\",\n          \"Intensity\": \"0\",\n          \"Wavelength\": \"0\"\n        }\n      ]\n    }\n  }\n}'),
(20, 1, 'Phalaenopsis3', '2017-09-11 20:09:46', 1, '{\n  \"Program\": {\n    \"DeviceName\": \"HBLUZ_1\",\n    \"ProgramName\": \"Phalaenopsis3\",\n    \"SyncTime\": \"20:08:57\",\n    \"SyncDate\": \"2017-08-09\",\n    \"ProgramDateStart\": \"2018-07-25\",\n    \"Lightsettings\": {\n      \"Lightsetting\": [\n        {\n          \"Description\": \"Morgens\",\n          \"RelativeStart\": 1,\n          \"Duration\": 182,\n          \"TimeStart\": \"00:00\",\n          \"TimeEnd\": \"00:00\",\n          \"Intensity\": \"0\",\n          \"Wavelength\": \"0\"\n        },\n        {\n          \"Description\": \"Unbenannt\",\n          \"RelativeStart\": 0,\n          \"Duration\": 162,\n          \"TimeStart\": \"00:00\",\n          \"TimeEnd\": \"00:00\",\n          \"Intensity\": \"0\",\n          \"Wavelength\": \"0\"\n        }\n      ]\n    }\n  }\n}');

-- --------------------------------------------------------

--
-- Tabellenstruktur für Tabelle `users`
--

CREATE TABLE `users` (
  `id` int(11) NOT NULL,
  `name` varchar(16) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Daten für Tabelle `users`
--

INSERT INTO `users` (`id`, `name`) VALUES
(1, 'Hans'),
(2, 'Patrick'),
(3, 'Peter'),
(4, 'Lisa'),
(5, 'Daniel');

--
-- Indizes der exportierten Tabellen
--

--
-- Indizes für die Tabelle `programs`
--
ALTER TABLE `programs`
  ADD PRIMARY KEY (`id`);

--
-- Indizes für die Tabelle `users`
--
ALTER TABLE `users`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `id` (`id`);

--
-- AUTO_INCREMENT für exportierte Tabellen
--

--
-- AUTO_INCREMENT für Tabelle `programs`
--
ALTER TABLE `programs`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=21;
--
-- AUTO_INCREMENT für Tabelle `users`
--
ALTER TABLE `users`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=6;COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
