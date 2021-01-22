USE `readsprofiler`;

DROP TABLE `books`; 

CREATE TABLE `books` (
	`isbn` varchar(13) NOT NULL,
    `title` varchar(100) NOT NULL,
    `year` int(4) NOT NULL,
    `pub_id` varchar(50) NOT NULL,
    `link` varchar(100) NOT NULL,
    `description` longtext NOT NULL,
    PRIMARY KEY (`isbn`)
);

DROP TABLE `book_points`;

CREATE TABLE `book_points` (
	`username` varchar(50) NOT NULL,
	`isbn` varchar(13) NOT NULL,
	`points` int NOT NULL DEFAULT 0,
    PRIMARY KEY (`username`, `isbn`)
);

DROP TABLE `authors`;

CREATE TABLE `authors` (
	`aut_code` varchar(5) NOT NULL,
    `first_name` varchar(50) NOT NULL,
    `last_name` varchar(50) NOT NULL,
    PRIMARY KEY (`aut_code`)
);

DROP TABLE `publications`;

CREATE TABLE `publications` (
	`isbn` varchar(13) NOT NULL,
    `aut_code` varchar(5) NOT NULL
);

DROP TABLE `genres`;

CREATE TABLE `genres` (
	`genre` varchar(50) NOT NULL,
    `sub_genre` varchar(50) NOT NULL
);

DROP TABLE `publishers`;

CREATE TABLE `publishers` (
	`pub_id` varchar(5) NOT NULL,
    `name` varchar(50) NOT NULL,
    PRIMARY KEY (`pub_id`)
);

DROP TABLE `book_genres`;

CREATE TABLE `book_genres` (
	`isbn` varchar(13) NOT NULL,
    `sub_genre` varchar(50) NOT NULL
);

DROP TABLE `clients`;

CREATE TABLE `clients` (
	`username` varchar(50) NOT NULL,
    `password` varchar(50) NOT NULL, 
    PRIMARY KEY (`username`)
); 

DROP TABLE `lists`;

CREATE TABLE `lists` (
    `username` varchar(50) NOT NULL,
    `type` enum('to_read', 'read') NOT NULL,
    `isbn` varchar(13) NOT NULL,
    `rating` int,
    CHECK (`rating` IN(1, 2, 3, 4, 5, NULL)),
    PRIMARY KEY (`username`, `isbn`)
);

INSERT INTO `books` VALUES('9780060291501', 'Alice\'s Adventures in Wonderland', 1865, 'pub01', 'http://bit.ly/Reads_Alice', 'It tells of a young girl named Alice, who falls through a rabbit hole into a subterranean fantasy world populated by peculiar, anthropomorphic creatures. It is considered to be one of the best examples of the literary nonsense genre. The tale plays with logic, giving the story lasting popularity with adults as well as with children.', '👧🏼🐰🎩');

INSERT INTO `books` VALUES('9780142412022', 'An Abundance of Katherines', 2006, 'pub02', 'http://bit.ly/Reads_Katherine', 'Colin Singleton, a child prodigy living in Chicago, fears he will not maintain his genius as an adult. Over the span of his life, Colin has dated nineteen girls named Katherine, all spelled in that manner. After being dumped by his girlfriend, Katherine XIX, Colin is longing to feel whole, and longing to matter. He hopes to become a genius by having a "eureka" moment.', '1️⃣9️⃣👧🏻');

INSERT INTO `books` VALUES('9780671502331', 'Catch 22', 1961, 'pub03', 'http://bit.ly/Reads_Catch', 'The horror begins with the attack on the undefended Italian mountain village, with the following chapters involving despair, disappearance in combat, disappearance caused by the army or death of most of Yossarian\'s friends, culminating in the horrors of Chapter 39, in particular the rape and murder of the innocent young woman, Michaela. In Chapter 41 the full details of the gruesome death of Snowden are finally revealed.', '🇮🇹🔪🏃🏻‍♂️');

INSERT INTO `books` VALUES('9781905716098', 'Complete Ghost Stories', 1904, 'pub04', 'http://bit.ly/Reads_Ghost', 'M. R. James wrote his ghost stories to entertain friends on Christmas Eve, and they went on to both transform and modernize a genre. James harnesses the power of suggestion to move from a recognizable world to one that is indefinably strange, and then unforgettably terrifying. Sheets, pictures, carvings, a doll\'s house, a lonely beach, a branch tapping on a window—ordinary things take on more than a tinge of dread in the hands of the original master of suspense.', '👻📚🏚');

INSERT INTO `books` VALUES('9780759568990', 'Dear John', 2006, 'pub05', 'http://bit.ly/Reads_John', 'Dear John is a romance novel by American writer Nicholas Sparks released in 2007. Its plot is an adaptation to present day\'s American culture of three plays Marius, Fanny and César, called la Trilogie Marseillaise written by French author Marcel Pagnol c. 1930. Sparks took inspiration from the real-life story of his cousin Todd Vance who served in the U.S. Armed Forces. It was on The New York Times Best Seller list in 2007.', '👮🏻‍♂️💔✈️');

INSERT INTO `books` VALUES('9780062387240', 'Divergent', 2001, 'pub01', 'http://bit.ly/Reads_Divergent', 'The novel Divergent features a post-apocalyptic version of Chicago and follows Beatrice "Tris" Prior as she explores her identity within a society that defines its citizens by their social and personality-related affiliation with five factions, which removes the threat of anyone exercising independent will and re-threatening the population\'s safety.', '🏋🏻‍♀️4️⃣⏱');

INSERT INTO `books` VALUES('9780765370624', 'Ender\'s Game', 1985, 'pub06', 'http://bit.ly/Reads_Ender', 'Set at an unspecified date in Earth\'s future, the novel presents an imperiled humankind after two conflicts with the Formics, an insectoid alien species they dub the "buggers". In preparation for an anticipated third invasion, children, including the novel\'s protagonist, Andrew "Ender" Wiggin, are trained from a very young age by putting them through increasingly difficult games, including some in zero gravity, where Ender\'s tactical genius is revealed.', '👨🏻‍🚀👾👽');

INSERT INTO `books` VALUES('9780143058137', 'Frankenstein', 1985, 'pub02', 'http://bit.ly/Reads_Frankenstein', 'Frankenstein; or, The Modern Prometheus is an 1818 novel written by English author Mary Shelley. Frankenstein tells the story of Victor Frankenstein, a young scientist who creates a sapient creature in an unorthodox scientific experiment. Shelley started writing the story when she was 18, and the first edition was published anonymously in London on 1 January 1818, when she was 20.', '🧟‍♂️🤪👨🏻‍⚕️');

INSERT INTO `books` VALUES('9780307237811', 'Genghis Khan and the Making of the Modern World', 2004, 'pub07', 'http://bit.ly/Reads_Genghis', 'Genghis Khan and the Making of the Modern World is a history book written by Jack Weatherford, Dewitt Wallace Professor of Anthropology at Macalester College. It is a narrative of the rise and influence of Genghis Khan and his successors, and their influence on European civilization.', '🌏🤴🏻⚔️');

INSERT INTO `books` VALUES('9780590353427', 'Harry Potter and the Sorcerer\'s Stone', 1997, 'pub08', 'http://bit.ly/Reads_Harry', 'The first novel in the Harry Potter series and Rowling\'s debut novel, it follows Harry Potter, a young wizard who discovers his magical heritage on his eleventh birthday, when he receives a letter of acceptance to Hogwarts School of Witchcraft and Wizardry. Harry makes close friends and a few enemies during his first year at the school, and with the help of his friends, Harry faces an attempted comeback by the dark wizard Lord Voldemort, who killed Harry\'s parents, but failed to kill Harry when he was just 15 months old.', '🪄👦🏻👓');

INSERT INTO `books` VALUES('9780062319791', 'I\'ll Be Gone in the Dark', 2018, 'pub01', 'http://bit.ly/Reads_Gone', 'I\'ll Be Gone in the Dark: One Woman\'s Obsessive Search for the Golden State Killer is a true crime book by Michelle McNamara about the Golden State Killer. The book was released on February 27, 2018, nearly two years after McNamara\'s death and two months before an arrest would be made in the case.', '🕵🏻‍♀️🔪☠️');

INSERT INTO `books` VALUES('9780141182575', 'In Cold Blood', 1967, 'pub02', 'http://bit.ly/Reads_Blood', 'In Cold Blood is a non-fiction novel, first published in 1966; it details the 1959 murders of four members of the Herbert Clutter family in the small farming community of Holcomb, Kansas.', '🔪🩸👨‍👩‍👧‍👦');

INSERT INTO `books` VALUES('9780141371207', 'Let It Snow', 2008, 'pub02', 'http://bit.ly/Reads_Snow', 'Let It Snow: Three Holiday Romances is a fix-up novel comprising three separate stories that intertwine with one another. It was released on 2 October 2008 through Speak. The stories are "The Jubilee Express" by Maureen Johnson, "A Cheertastic Christmas Miracle" by John Green, and "The Patron Saint of Pigs" by Lauren Myracle. The book follows three different teenagers as they experience a huge snow storm in the town of Gracetown during the Christmas season.', '📣🚞❄️');

INSERT INTO `books` VALUES('9780739377956', 'Life of Pi', 2001, 'pub02', 'http://bit.ly/Reads_Pi', 'The protagonist is Piscine Molitor "Pi" Patel, an Indian Hindu boy from Pondicherry who explores issues of spirituality and metaphysics from an early age. He survives 227 days after a shipwreck while stranded on a lifeboat in the Pacific Ocean with a Bengal tiger which raises questions about the nature of reality and how it is perceived and told.', '🐅👳🏽‍♂️🛶');

INSERT INTO `books` VALUES('9780547928210', 'Lord of the Rings - The Fellowship of the Ring', 1954, 'pub09', 'http://bit.ly/Reads_Lord', 'Bilbo celebrates his 111th birthday and leaves the Shire, leaving the Ring to Frodo Baggins, his cousin and heir. Neither hobbit is aware of the Ring\'s nature, but the wizard Gandalf realises that it is a Ring of Power. Seventeen years later, Gandalf tells Frodo that he has confirmed that the Ring is the one lost by the Dark Lord Sauron long ago and counsels him to take it away from the Shire.', '🐉💍🧝🏻');

INSERT INTO `books` VALUES('9780142402535', 'Matilda', 1996, 'pub10', 'http://bit.ly/Reads_Matilda', 'Story of a wonderful little girl, who happens to be a genius, and her wonderful teacher vs. the worst parents ever and the worst school principal imaginable.', '👧🏻👺🏫');

INSERT INTO `books` VALUES('9781419701764', 'Me and Earl and the Dying Girl', 2015, 'pub11', 'http://bit.ly/Reads_Earl', 'It is a universally acknowledged truth that high school sucks. But on the first day of his senior year, Greg Gaines thinks he’s figured it out. The answer to the basic existential question: How is it possible to exist in a place that sucks so bad? His strategy: remain at the periphery at all times. Make mediocre films with the one person who is even sort of his friend, Earl.', '👦🏻🎥🏥');

INSERT INTO `books` VALUES('9781234567890', 'Mihai Eminescu in English Collection', 1883, 'pub12', 'http://bit.ly/Reads_Eminescu', 'Mihai Eminescu was a Romanian Romantic poet, novelist, and journalist, generally regarded as the most famous and influential Romanian poet. He was an active member of the Junimea literary society and worked as an editor for the newspaper "The Time". His poetry was first published when he was 16 and he went to Vienna to study when he was 19. The poet\'s manuscripts, containing 46 volumes and approximately 14,000 pages.', '🌟🇷🇴📜');

INSERT INTO `books` VALUES('9780062073501', 'Murder on the Orient Express', 1934, 'pub01', 'http://bit.ly/Reads_Orient', 'Just after midnight, the famous Orient Express is stopped in its tracks by a snowdrift. By morning, the millionaire Samuel Edward Ratchett lies dead in his compartment, stabbed a dozen times, his door locked from the inside. One of his fellow passengers must be the murderer. Isolated by the storm, detective Hercule Poirot must find the killer among a dozen of men.', '🚂🕵🏻‍♂️🔪');

INSERT INTO `books` VALUES('9780012345678', 'Praise the Rain', 2015, 'pub08', 'http://bit.ly/Reads_Rain', 'The most representative poem from the Confilct Resolution for Holy Beings volume.', '🌧👧🏻🙏🏻');

INSERT INTO `books` VALUES('9780307887436', 'Ready Player One', 2011, 'pub07', 'http://bit.ly/Reads_Ready', 'The story, set in a dystopia in 2045, follows protagonist Wade Watts on his search for an Easter egg in a worldwide virtual reality game, the discovery of which would lead him to inherit the game creator\'s fortune.', '🎮1️⃣🥚');

INSERT INTO `books` VALUES('9780393964523', 'Robinson Crusoe', 1719, 'pub04', 'http://bit.ly/Reads_Robinson', 'The first edition credited the work\'s protagonist Robinson Crusoe as its author, leading many readers to believe he was a real person and the book a travelogue of true incidents. Epistolary, confessional, and didactic in form, the book is presented as an autobiography of the title character (whose birth name is Robinson Kreutznaer) – a castaway who spends 28 years on a remote tropical desert island near the coasts of Venezuela and Trinidad.', '🏝👨🏾⛺️');

INSERT INTO `books` VALUES('9780391234567', 'Siren Song', 2002, 'pub10', 'http://bit.ly/Reads_Siren', 'The most representative poem from the Selected Poems volume.', '🧜🏻‍♀🎶⚓️');

INSERT INTO `books` VALUES('9781442453937', 'Steve Jobs - Thinking Differently', 2012, 'pub02', 'http://bit.ly/Reads_Steve', 'Visionary. Pioneer. Little terror. Entrepreneur. Inventor. College dropout. Creative genius. These are just a few of the words used to describe the late Steve Jobs, cofounder, chairman, and chief executive officer of Apple Inc.', '🍏📱🐢');

INSERT INTO `books` VALUES('9780062301253', 'Tesla, SpaceX, and the Quest for a Fantastic Future', 2017, 'pub13', 'http://bit.ly/Reads_Elon', 'Elon Musk: Tesla, SpaceX, and the Quest for a Fantastic Future is Ashlee Vance\'s biography of Elon Musk, published in 2015. The book traces Elon Musk\'s life from his childhood up to the time he spent at Zip2 and PayPal, and then onto SpaceX, Tesla, and SolarCity.', '🚙🚀💸');

INSERT INTO `books` VALUES('9781976467981', 'The Call of Cthulhu', 1928, 'pub09', 'http://bit.ly/Reads_Cthulhu', 'Written in the summer of 1926, it was first published in the pulp magazine Weird Tales in February 1928. The Call of Cthulhu is a harrowing tale of the weakness of the human mind when confronted by powers and intelligences from beyond our world.', '🐙👹📞');

INSERT INTO `books` VALUES('9780810993136', 'The Diary of a Wimpy Kid', 2007, 'pub10', 'http://bit.ly/Reads_Wimpy', 'All the main books are the journals of the main character, Greg Heffley. Befitting a teen\'s diary, the books are filled with simple drawings of Greg\'s daily adventures. Since the release of the online version in May 2004, most of the books have garnered positive reviews and commercial success. As of 2020, more than 250 million copies have been sold globally.', '👦🏻📔🏫');

INSERT INTO `books` VALUES('9780141039824', 'The Diary of a Young Girl', 1947, 'pub02', 'http://bit.ly/Reads_Diary', 'The Diary of a Young Girl, also known as The Diary of Anne Frank, is a book of the writings from the Dutch-language diary kept by Anne Frank while she was in hiding for two years with her family during the Nazi occupation of the Netherlands. The family was apprehended in 1944, and Anne Frank died of typhus in the Bergen-Belsen concentration camp in 1945. The diary was retrieved by Miep Gies, who gave it to Anne\'s father, Otto Frank, the family\'s only known survivor, just after the war was over.', '👧🏻🇩🇪😔');

INSERT INTO `books` VALUES('9780142424179', 'The Fault in Our Stars', 2012, 'pub02', 'http://bit.ly/Reads_Stars', 'The story is narrated by Hazel Grace Lancaster, a 16-year-old girl with thyroid cancer that has affected her lungs. Hazel is forced by her parents to attend a support group where she subsequently meets and falls in love with 17-year-old Augustus Waters, an ex-basketball player and amputee.', '👩🏻👨🏻✨');

INSERT INTO `books` VALUES('9780143122357', 'The Haunting of Hill House', 1959, 'pub02', 'http://bit.ly/Reads_Haunting', 'A finalist for the National Book Award and considered one of the best literary ghost stories published during the 20th century, it has been made into two feature films and a play, and is the basis of a Netflix series. Jackson\'s novel relies on terror rather than horror to elicit emotion in the reader, using complex relationships between the mysterious events in the house and the characters’ psyches.', '🏠👻🏔');

INSERT INTO `books` VALUES('9780330508117', 'The Hitchhiker\'s Guide to the Galaxy', 1979, 'pub14', 'http://bit.ly/Reads_Galaxy', 'The Hitchhiker\'s Guide to the Galaxy is the first of six books in the Hitchhiker\'s Guide to the Galaxy comedy science fiction "trilogy" by Douglas Adams. The novel is an adaptation of the first four parts of Adams\'s radio series of the same name. It sold 250,000 copies in the first three months.', '🌌🚗📖');

INSERT INTO `books` VALUES('9780439023528', 'The Hunger Games', 2008, 'pub08', 'http://bit.ly/Reads_Hunger', 'It is written in the voice of 16-year-old Katniss Everdeen, who lives in the future, post-apocalyptic nation of Panem in North America. The Capitol, a highly advanced metropolis, exercises political control over the rest of the nation. The Hunger Games is an annual event in which one boy and one girl aged 12–18 from each of the twelve districts surrounding the Capitol are selected by lottery to compete in a televised battle royale to the death.', '🍽🎮🏹');

INSERT INTO `books` VALUES('9781784874186', 'The Little Prince', 1943, 'pub02', 'http://bit.ly/Reads_Prince', 'The Little Prince is a novel by French aristocrat, writer, and aviator Antoine de Saint-Exupéry. The story follows a young prince who visits various planets in space, including Earth, and addresses themes of loneliness, friendship, love, and loss. Despite its style as a children\'s book, The Little Prince makes observations about life and human nature.', '👦🏼🌹🐘');

INSERT INTO `books` VALUES('9780385737951', 'The Maze Runner', 2009, 'pub11', 'http://bit.ly/Reads_Maze', 'Thomas is deposited in a community of boys after his memory is erased, soon learning they\'re all trapped in a maze that will require him to join forces with fellow "runners" for a shot at escape.', '🏃🏻‍♂️🏔🤷🏻');

INSERT INTO `books` VALUES('9780751556896', 'The Notebook', 1996, 'pub15', 'http://bit.ly/Reads_Notebook', 'A poor yet passionate young man falls in love with a rich young woman, giving her a sense of freedom, but they are soon separated because of their social differences.', '❤️📓👵🏻');

INSERT INTO `books` VALUES('9780751123456', 'The Raven', 1845, 'pub08', 'http://bit.ly/Reads_Raven', 'First published in January 1845, the poem is often noted for its musicality, stylized language, and supernatural atmosphere. It tells of a talking raven\'s mysterious visit to a distraught lover, tracing the man\'s slow descent into madness. The lover, often identified as a student, is lamenting the loss of his love, Lenore.', '🦉📜💔');

INSERT INTO `books` VALUES('9780300234077', 'The Story of My Experiments with Truth', 1927, 'pub16', 'http://bit.ly/Reads_Gandhi', 'The independence of India was a long and difficult process, led by the activist Mohandas Gandhi. In his book , he narrates his trajectory in the fight against English colonialism and the dissemination of his philosophy, known as “Satyagraha”.', '🧘🏾‍♂️🧠🇮🇳');

INSERT INTO `books` VALUES('9780192728029', 'The Wonderful Wizard of Oz', 1900, 'pub17', 'http://bit.ly/Reads_Oz', 'The story chronicles the adventures of a young farm girl named Dorothy in the magical Land of Oz after she and her pet dog Toto are swept away from their Kansas home by a cyclone. The book is one of the best-known stories in American literature and has been widely translated.', '🧙🏻‍♂️🦁🤖');

INSERT INTO `books` VALUES('9780425212738', 'Zodiac Unmasked', 1986, 'pub18', 'http://bit.ly/Reads_Zodiac', 'Zodiac is a non-fiction book written by Robert Graysmith about the unsolved serial murders committed by the "Zodiac Killer" in San Francisco in the late 1960s and early \'70s. Since its initial release in 1986, Zodiac has sold 4 million copies worldwide. Graysmith was a cartoonist for the San Francisco Chronicle and later also wrote Zodiac Unmasked.', '♌️🎭🔪');

INSERT INTO `authors` VALUES('aut01', 'Lewis', 'Carroll');
INSERT INTO `authors` VALUES('aut02', 'John', 'Green');
INSERT INTO `authors` VALUES('aut03', 'Joseph', 'Heller');
INSERT INTO `authors` VALUES('aut04', 'M. R.', 'James');
INSERT INTO `authors` VALUES('aut05', 'Nicholas', 'Sparks');
INSERT INTO `authors` VALUES('aut06', 'Veronica', 'Roth');
INSERT INTO `authors` VALUES('aut07', 'Orson Scott', 'Card');
INSERT INTO `authors` VALUES('aut08', 'Mary', 'Shelley');
INSERT INTO `authors` VALUES('aut09', 'Jack', 'Weatherford');
INSERT INTO `authors` VALUES('aut10', 'J. K.', 'Rowling');
INSERT INTO `authors` VALUES('aut11', 'Michelle', 'McNamara');
INSERT INTO `authors` VALUES('aut12', 'Truman', 'Capote');
INSERT INTO `authors` VALUES('aut13', 'Maureen', 'Johnson');
INSERT INTO `authors` VALUES('aut14', 'Lauren', 'Myracle');
INSERT INTO `authors` VALUES('aut15', 'Yann', 'Martel');
INSERT INTO `authors` VALUES('aut16', 'J. R. R.', 'Tolkien');
INSERT INTO `authors` VALUES('aut17', 'Roald', 'Dahl');
INSERT INTO `authors` VALUES('aut18', 'Quentin', 'Blake');
INSERT INTO `authors` VALUES('aut19', 'Jesse', 'Andrews');
INSERT INTO `authors` VALUES('aut20', 'Mihai', 'Eminescu');
INSERT INTO `authors` VALUES('aut21', 'Agatha', 'Christie');
INSERT INTO `authors` VALUES('aut22', 'Joy', 'Harjo');
INSERT INTO `authors` VALUES('aut23', 'Ernest', 'Cline');
INSERT INTO `authors` VALUES('aut24', 'Daniel', 'Defoe');
INSERT INTO `authors` VALUES('aut25', 'Margaret', 'Atwood');
INSERT INTO `authors` VALUES('aut26', 'Patricia', 'Lakin');
INSERT INTO `authors` VALUES('aut27', 'Ashlee', 'Vance');
INSERT INTO `authors` VALUES('aut28', 'H. P.', 'Lovecraft');
INSERT INTO `authors` VALUES('aut29', 'Jeff', 'Kinney');
INSERT INTO `authors` VALUES('aut30', 'Anne', 'Frank');
INSERT INTO `authors` VALUES('aut31', 'Shirley', 'Jackson');
INSERT INTO `authors` VALUES('aut32', 'Douglas', 'Adams');
INSERT INTO `authors` VALUES('aut33', 'Suzanne', 'Collins');
INSERT INTO `authors` VALUES('aut34', 'Antoine', 'de Saint-Exupery');
INSERT INTO `authors` VALUES('aut35', 'James', 'Dashner');
INSERT INTO `authors` VALUES('aut36', 'Edgar Allan', 'Poe');
INSERT INTO `authors` VALUES('aut37', 'Mahatma K.', 'Gandhi');
INSERT INTO `authors` VALUES('aut38', 'L. Frank', 'Baum');
INSERT INTO `authors` VALUES('aut39', 'Robert', 'Graysmith');

INSERT INTO `publications` VALUES('9780060291501', 'aut01');
INSERT INTO `publications` VALUES('9780142412022', 'aut02');
INSERT INTO `publications` VALUES('9780141371207', 'aut02');
INSERT INTO `publications` VALUES('9780142424179', 'aut02');
INSERT INTO `publications` VALUES('9780671502331', 'aut03');
INSERT INTO `publications` VALUES('9781905716098', 'aut04');
INSERT INTO `publications` VALUES('9780759568990', 'aut05');
INSERT INTO `publications` VALUES('9780751556896', 'aut05');
INSERT INTO `publications` VALUES('9780062387240', 'aut06');
INSERT INTO `publications` VALUES('9780765370624', 'aut07');
INSERT INTO `publications` VALUES('9780143058137', 'aut08');
INSERT INTO `publications` VALUES('9780307237811', 'aut09');
INSERT INTO `publications` VALUES('9780590353427', 'aut10');
INSERT INTO `publications` VALUES('9780062319791', 'aut11');
INSERT INTO `publications` VALUES('9780141182575', 'aut12');
INSERT INTO `publications` VALUES('9780141371207', 'aut13');
INSERT INTO `publications` VALUES('9780141371207', 'aut14');
INSERT INTO `publications` VALUES('9780739377956', 'aut15');
INSERT INTO `publications` VALUES('9780547928210', 'aut16');
INSERT INTO `publications` VALUES('9780142402535', 'aut17');
INSERT INTO `publications` VALUES('9780142402535', 'aut18');
INSERT INTO `publications` VALUES('9781419701764', 'aut19');
INSERT INTO `publications` VALUES('9781234567890', 'aut20');
INSERT INTO `publications` VALUES('9780062073501', 'aut21');
INSERT INTO `publications` VALUES('9780012345678', 'aut22');
INSERT INTO `publications` VALUES('9780307887436', 'aut23');
INSERT INTO `publications` VALUES('9780393964523', 'aut24');
INSERT INTO `publications` VALUES('9780391234567', 'aut25');
INSERT INTO `publications` VALUES('9781442453937', 'aut26');
INSERT INTO `publications` VALUES('9780062301253', 'aut27');
INSERT INTO `publications` VALUES('9781976467981', 'aut28');
INSERT INTO `publications` VALUES('9780810993136', 'aut29');
INSERT INTO `publications` VALUES('9780141039824', 'aut30');
INSERT INTO `publications` VALUES('9780143122357', 'aut31');
INSERT INTO `publications` VALUES('9780330508117', 'aut32');
INSERT INTO `publications` VALUES('9780439023528', 'aut33');
INSERT INTO `publications` VALUES('9781784874186', 'aut34');
INSERT INTO `publications` VALUES('9780385737951', 'aut35');
INSERT INTO `publications` VALUES('9780751123456', 'aut36');
INSERT INTO `publications` VALUES('9780300234077', 'aut37');
INSERT INTO `publications` VALUES('9780192728029', 'aut38');
INSERT INTO `publications` VALUES('9780425212738', 'aut39');

INSERT INTO `genres` VALUES('Action', 'Rebellion');
INSERT INTO `genres` VALUES('Action', 'Survival');
INSERT INTO `genres` VALUES('Action', 'Thriller');

INSERT INTO `genres` VALUES('Adventure', 'Group Adventure');
INSERT INTO `genres` VALUES('Adventure', 'Solo Adventure');

INSERT INTO `genres` VALUES('Biographical', 'Autobiography');
INSERT INTO `genres` VALUES('Biographical', 'Biography');

INSERT INTO `genres` VALUES('Children', 'Slice of Life');
INSERT INTO `genres` VALUES('Children', 'Journey');

INSERT INTO `genres` VALUES('Comedy', 'Dark Humor');
INSERT INTO `genres` VALUES('Comedy', 'Cringe Comedy');

INSERT INTO `genres` VALUES('Crime', 'Detective');
INSERT INTO `genres` VALUES('Crime', 'Serial Killer');

INSERT INTO `genres` VALUES('Drama', 'Castaway');
INSERT INTO `genres` VALUES('Drama', 'Love');

INSERT INTO `genres` VALUES('Fantasy', 'Magic');

INSERT INTO `genres` VALUES('History', 'Real');
INSERT INTO `genres` VALUES('History', 'Fiction');

INSERT INTO `genres` VALUES('Horror', 'Monster');
INSERT INTO `genres` VALUES('Horror', 'Paranormal');

INSERT INTO `genres` VALUES('Poetry', 'Classic');
INSERT INTO `genres` VALUES('Poetry', 'Contemporary');

INSERT INTO `genres` VALUES('Romance', 'Teen');
INSERT INTO `genres` VALUES('Romance', 'Adult');

INSERT INTO `genres` VALUES('Science-Fiction', 'Dystopian');
INSERT INTO `genres` VALUES('Science-Fiction', 'Military');

INSERT INTO `publishers` VALUES('pub01', 'Harper Collins');
INSERT INTO `publishers` VALUES('pub02', 'Penguin Random House');
INSERT INTO `publishers` VALUES('pub03', 'Simon & Schuster');
INSERT INTO `publishers` VALUES('pub04', 'Collector\'s Library');
INSERT INTO `publishers` VALUES('pub05', 'Warner Books');
INSERT INTO `publishers` VALUES('pub06', 'Tor Books');
INSERT INTO `publishers` VALUES('pub07', 'Crown Publishing Group');
INSERT INTO `publishers` VALUES('pub08', 'Scholastic');
INSERT INTO `publishers` VALUES('pub09', 'Mariner Books');
INSERT INTO `publishers` VALUES('pub10', 'Puffin Books');
INSERT INTO `publishers` VALUES('pub11', 'Abrams Books');
INSERT INTO `publishers` VALUES('pub12', 'Editura Litera');
INSERT INTO `publishers` VALUES('pub13', 'Ecco');
INSERT INTO `publishers` VALUES('pub14', 'MacMillan');
INSERT INTO `publishers` VALUES('pub15', 'Sphere');
INSERT INTO `publishers` VALUES('pub16', 'Yale University Press');
INSERT INTO `publishers` VALUES('pub17', 'Oxford University Press');
INSERT INTO `publishers` VALUES('pub18', 'Berkley University Press');

INSERT INTO `book_genres` VALUES('9780060291501', 'Journey');
INSERT INTO `book_genres` VALUES('9780142412022', 'Teen');
INSERT INTO `book_genres` VALUES('9780671502331', 'Dark Humor');
INSERT INTO `book_genres` VALUES('9781905716098', 'Paranormal');
INSERT INTO `book_genres` VALUES('9780759568990', 'Adult');
INSERT INTO `book_genres` VALUES('9780759568990', 'Love');
INSERT INTO `book_genres` VALUES('9780062387240', 'Rebellion');
INSERT INTO `book_genres` VALUES('9780062387240', 'Dystopian');
INSERT INTO `book_genres` VALUES('9780765370624', 'Military');
INSERT INTO `book_genres` VALUES('9780143058137', 'Monster');
INSERT INTO `book_genres` VALUES('9780590353427', 'Group Adventure');
INSERT INTO `book_genres` VALUES('9780590353427', 'Magic');
INSERT INTO `book_genres` VALUES('9780307237811', 'Real');
INSERT INTO `book_genres` VALUES('9780062319791', 'Detective');
INSERT INTO `book_genres` VALUES('9780141182575', 'Serial Killer');
INSERT INTO `book_genres` VALUES('9780141371207', 'Teen');
INSERT INTO `book_genres` VALUES('9780739377956', 'Solo Adventure');
INSERT INTO `book_genres` VALUES('9780739377956', 'Castaway');
INSERT INTO `book_genres` VALUES('9780739377956', 'Fiction');
INSERT INTO `book_genres` VALUES('9780547928210', 'Group Adventure');
INSERT INTO `book_genres` VALUES('9780547928210', 'Magic');
INSERT INTO `book_genres` VALUES('9780142402535', 'Slice of Life');
INSERT INTO `book_genres` VALUES('9781419701764', 'Cringe Comedy');
INSERT INTO `book_genres` VALUES('9781419701764', 'Dark Humor');
INSERT INTO `book_genres` VALUES('9781234567890', 'Classic');
INSERT INTO `book_genres` VALUES('9780062073501', 'Detective');
INSERT INTO `book_genres` VALUES('9780012345678', 'Contemporary');
INSERT INTO `book_genres` VALUES('9780307887436', 'Rebellion');
INSERT INTO `book_genres` VALUES('9780307887436', 'Dystopian');
INSERT INTO `book_genres` VALUES('9780393964523', 'Solo Adventure');
INSERT INTO `book_genres` VALUES('9780393964523', 'Fiction');
INSERT INTO `book_genres` VALUES('9780391234567', 'Contemporary');
INSERT INTO `book_genres` VALUES('9781442453937', 'Biography');
INSERT INTO `book_genres` VALUES('9780062301253', 'Biography');
INSERT INTO `book_genres` VALUES('9781976467981', 'Monster');
INSERT INTO `book_genres` VALUES('9780810993136', 'Slice of Life');
INSERT INTO `book_genres` VALUES('9780141039824', 'Autobiography');
INSERT INTO `book_genres` VALUES('9780141039824', 'Real');
INSERT INTO `book_genres` VALUES('9780142424179', 'Teen');
INSERT INTO `book_genres` VALUES('9780142424179', 'Love');
INSERT INTO `book_genres` VALUES('9780143122357', 'Paranormal');
INSERT INTO `book_genres` VALUES('9780330508117', 'Dark Humor');
INSERT INTO `book_genres` VALUES('9780439023528', 'Survival');
INSERT INTO `book_genres` VALUES('9780439023528', 'Rebellion');
INSERT INTO `book_genres` VALUES('9780439023528', 'Dystopian');
INSERT INTO `book_genres` VALUES('9781784874186', 'Journey');
INSERT INTO `book_genres` VALUES('9780385737951', 'Survival');
INSERT INTO `book_genres` VALUES('9780385737951', 'Thriller');
INSERT INTO `book_genres` VALUES('9780385737951', 'Dystopian');
INSERT INTO `book_genres` VALUES('9780751556896', 'Love');
INSERT INTO `book_genres` VALUES('9780751556896', 'Adult');
INSERT INTO `book_genres` VALUES('9780751123456', 'Classic');
INSERT INTO `book_genres` VALUES('9780300234077', 'Autobiography');
INSERT INTO `book_genres` VALUES('9780192728029', 'Journey');
INSERT INTO `book_genres` VALUES('9780425212738', 'Serial Killer');










