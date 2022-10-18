-- Keep a log of any SQL queries you execute as you solve the mystery.

-- First, I wanted to know the crime scene reports
SELECT * FROM crime_scene_reports;
-- HINT: id = 295;

-- Looked at the interviews of the theft day
SELECT * FROM interviews WHERE month = 7 AND day = 28;
-- HINT: id = 161, 162 and 163

-- Looked for the license_plates that came out close to the time of the theft
SELECT * FROM bakery_security_logs WHERE month = 7 AND day = 28 AND hour = 10;
--+-----+------+-------+-----+------+--------+----------+---------------+
--| id  | year | month | day | hour | minute | activity | license_plate |
--+-----+------+-------+-----+------+--------+----------+---------------+
--| 258 | 2021 | 7     | 28  | 10   | 8      | entrance | R3G7486       |
--| 259 | 2021 | 7     | 28  | 10   | 14     | entrance | 13FNH73       |
--| 260 | 2021 | 7     | 28  | 10   | 16     | exit     | 5P2BI95       |
--| 261 | 2021 | 7     | 28  | 10   | 18     | exit     | 94KL13X       |
--| 262 | 2021 | 7     | 28  | 10   | 18     | exit     | 6P58WS2       |
--| 263 | 2021 | 7     | 28  | 10   | 19     | exit     | 4328GD8       |
--| 264 | 2021 | 7     | 28  | 10   | 20     | exit     | G412CB7       |
--| 265 | 2021 | 7     | 28  | 10   | 21     | exit     | L93JTIZ       |
--| 266 | 2021 | 7     | 28  | 10   | 23     | exit     | 322W7JE       |
--| 267 | 2021 | 7     | 28  | 10   | 23     | exit     | 0NTHK55       |
--| 268 | 2021 | 7     | 28  | 10   | 35     | exit     | 1106N58       |
--| 269 | 2021 | 7     | 28  | 10   | 42     | entrance | NRYN856       |
--| 270 | 2021 | 7     | 28  | 10   | 44     | entrance | WD5M8I6       |
--| 271 | 2021 | 7     | 28  | 10   | 55     | entrance | V47T75I       |
--+-----+------+-------+-----+------+--------+----------+---------------+

-- (BASED ON INTERVIEW 161) People who left the place at 10 o'clock
SELECT * FROM people WHERE license_plate IN (SELECT license_plate FROM bakery_security_logs WHERE id BETWEEN 260 AND 268);
--+--------+---------+----------------+-----------------+---------------+
--|   id   |  name   |  phone_number  | passport_number | license_plate |
--+--------+---------+----------------+-----------------+---------------+
--| 221103 | Vanessa | (725) 555-4692 | 2963008352      | 5P2BI95       |
--| 243696 | Barry   | (301) 555-4174 | 7526138472      | 6P58WS2       |
--| 396669 | Iman    | (829) 555-5269 | 7049073643      | L93JTIZ       |
--| 398010 | Sofia   | (130) 555-0289 | 1695452385      | G412CB7       |
--| 449774 | Taylor  | (286) 555-6063 | 1988161715      | 1106N58       |
--| 467400 | Luca    | (389) 555-5198 | 8496433585      | 4328GD8       |
--| 514354 | Diana   | (770) 555-1861 | 3592750733      | 322W7JE       |
--| 560886 | Kelsey  | (499) 555-9472 | 8294398571      | 0NTHK55       |
--| 686048 | Bruce   | (367) 555-5533 | 5773159633      | 94KL13X       |
--+--------+---------+----------------+-----------------+---------------+

-- (BASED ON INTERVIEW 163) People who left the place at 10 o'clock and took a flight the next day
SELECT pe.name FROM people AS pe
JOIN passengers AS pa ON pe.passport_number = pa.passport_number
JOIN flights AS fl ON pa.flight_id = fl.id
WHERE fl.month = 7 AND fl.day = 29 AND pe.name
IN (SELECT name FROM people WHERE license_plate IN (SELECT license_plate FROM bakery_security_logs WHERE id BETWEEN 260 AND 268));
--+--------+
--|  name  |
--+--------+
--| Diana  |
--| Sofia  |
--| Bruce  |
--| Kelsey |
--| Taylor |
--| Luca   |
--+--------+

SELECT * FROM atm_transactions
WHERE month = 7
AND day = 28
AND atm_location = "Leggett Street";
--+-----+----------------+------+-------+-----+----------------+------------------+--------+
--| id  | account_number | year | month | day |  atm_location  | transaction_type | amount |
--+-----+----------------+------+-------+-----+----------------+------------------+--------+
--| 246 | 28500762       | 2021 | 7     | 28  | Leggett Street | withdraw         | 48     |
--| 264 | 28296815       | 2021 | 7     | 28  | Leggett Street | withdraw         | 20     |
--| 266 | 76054385       | 2021 | 7     | 28  | Leggett Street | withdraw         | 60     |
--| 267 | 49610011       | 2021 | 7     | 28  | Leggett Street | withdraw         | 50     |
--| 269 | 16153065       | 2021 | 7     | 28  | Leggett Street | withdraw         | 80     |
--| 275 | 86363979       | 2021 | 7     | 28  | Leggett Street | deposit          | 10     |
--| 288 | 25506511       | 2021 | 7     | 28  | Leggett Street | withdraw         | 20     |
--| 313 | 81061156       | 2021 | 7     | 28  | Leggett Street | withdraw         | 30     |
--| 336 | 26013199       | 2021 | 7     | 28  | Leggett Street | withdraw         | 35     |
--+-----+----------------+------+-------+-----+----------------+------------------+--------+

-- (BASED ON INTERVIEW 162) People who withdrew the money on Legget Street
SELECT people.name
FROM people
JOIN bank_accounts ON people.id = bank_accounts.person_id
JOIN atm_transactions ON bank_accounts.account_number = atm_transactions.account_number
WHERE month = 7
AND day = 28
AND atm_location = "Leggett Street";
--+---------+
--|  name   |
--+---------+
--| Bruce   |
--| Kaelyn  |
--| Diana   |
--| Brooke  |
--| Kenny   |
--| Iman    |
--| Luca    |
--| Taylor  |
--| Benista |
--+---------+

-- (BASED ON INTERVIEWS 161, 162 and 163)
SELECT people.name
FROM people
JOIN bank_accounts ON people.id = bank_accounts.person_id
JOIN atm_transactions ON bank_accounts.account_number = atm_transactions.account_number
WHERE month = 7
AND day = 28
AND atm_location = "Leggett Street"
AND people.name
IN (SELECT pe.name FROM people AS pe
JOIN passengers AS pa ON pe.passport_number = pa.passport_number
JOIN flights AS fl ON pa.flight_id = fl.id
WHERE fl.month = 7 AND fl.day = 29 AND pe.name
IN (SELECT name FROM people WHERE license_plate IN (SELECT license_plate FROM bakery_security_logs WHERE id BETWEEN 260 AND 268)));
--+--------+
--|  name  |
--+--------+
--| Bruce  | but something tells me that Bruce, caught the duck and was the only one who took 50 bucks and this is cs50 so...
--| Diana  |
--| Luca   |
--| Taylor |
--+--------+

SELECT * FROM people
WHERE name = "Bruce";
--+--------+-------+----------------+-----------------+---------------+
--|   id   | name  |  phone_number  | passport_number | license_plate |
--+--------+-------+----------------+-----------------+---------------+
--| 686048 | Bruce | (367) 555-5533 | 5773159633      | 94KL13X       |
--+--------+-------+----------------+-----------------+---------------+

SELECT * FROM phone_calls
WHERE caller = "(367) 555-5533"
AND day = 28
AND duration < 60;
--+-----+----------------+----------------+------+-------+-----+----------+
--| id  |     caller     |    receiver    | year | month | day | duration |
--+-----+----------------+----------------+------+-------+-----+----------+
--| 233 | (367) 555-5533 | (375) 555-8161 | 2021 | 7     | 28  | 45       |
--+-----+----------------+----------------+------+-------+-----+----------+

SELECT * FROM people
WHERE phone_number = "(375) 555-8161";
--+--------+-------+----------------+-----------------+---------------+
--|   id   | name  |  phone_number  | passport_number | license_plate |
--+--------+-------+----------------+-----------------+---------------+
--| 864400 | Robin | (375) 555-8161 |                 | 4V16VO0       |
--+--------+-------+----------------+-----------------+---------------+

SELECT airports.city FROM airports
JOIN flights ON airports.id = flights.destination_airport_id
JOIN passengers ON flights.id = passengers.flight_id
JOIN people ON passengers.passport_number = people.passport_number
WHERE people.passport_number = "5773159633";
--+---------------+
--|     city      |
--+---------------+
--| New York City |
--+---------------+