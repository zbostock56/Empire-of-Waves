#include <stdio.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <globals.h>

#define INVASION_REP (-25.0)
#define TOP (0)
#define BOTTOM (1)
#define LEFT (2)
#define RIGHT (3)

/* Names of the merchants that can be chosen from */
char merchant_name_list[NUM_NAMES][NAME_SIZE] = {
"Phytheon", "Moeris", "Hegetoridas", "Juantxiki", "Aberardus",
"Droet", "Garikoitz", "Goisfridus", "Kameirus", "Amyris",
"Jep", "Acestes", "Basan", "Aldebrand", "Ulricus", "Rabel",
"Alisandre", "Phil", "Thim", "Borchert",
"Bidun", "Snazi", "Owin", "Amalrich", "Melampus",
"Eurytos", "Gawyn", "Thyestes", "Pariset", "Carpus",
"Martyrius", "Poppe", "Pylades", "Aristaios", "Democedes",
"Jaime", "Atzo", "Remia", "Dithyrambos", "Tzittas",
"Therapon", "Thrasymedes", "Phillipos", "Crispinian",
"Flambard", "Azzo", "Ger", "Hauwe", "Abreas", "Borchert",
"Thurstin", "Herould", "Arnaut", "Pallas", "Otois", "Rankin",
"Unai", "Kapys", "Hokianga", "Stesanor",
"Filibert", "Aristaeus", "Gagino", "Fulbertus", "Degarre",
"Milo", "Nisos", "Jaizki", "Carellus", "Hewlett",
"Dennis", "Ollier", "Neoptolemos", "Walahmar", "Alcaeos",
"Richomeres", "Sesuldo", "Averitt", "Lichas", "Pratinos",
"Biche", "Diores", "Wasili", "Piarres", "Sigibert", "Herry",
"Girout", "Ortwinus", "Haritzeder", "Theodric",
"Macar", "Vacho", "Cheldric", "Reimund", "Gallus", "Osewold",
"Authari", "Wultgar", "Drue", "Iphitos",
"Laios", "Faramund", "Adalbert", "Frideric", "Odovacar", "Karolus",
"Gallien", "Richomeres", "Dreues", "Waihoroi",
"Pyrrhus", "Uctred", "Zumar", "Hardi", "Imbrius", "Bert",
"Floris", "Michell", "Fehde", "Gobert",
"Gelon", "Podalinus", "Anfroi", "Bias", "Ion", "Aristarchos",
"Tenthredon", "Gentien", "Hippoklides", "Meme",
"Imgelramus", "Tisandros", "Godfree", "Sohier", "Tadica",
"Rolland", "Curteis", "Anaut", "Metiochus", "Reimfred",
"Phalanthus", "Eniopus", "Astell", "Mamme", "Iparragirre",
"Jollanus", "Alex", "Kleitos", "Rasequin", "Roucaud",
"Bidari", "Aleaume", "Seguin", "Guinemant", "Morcock",
"Waldipert", "Phormion", "Jibbo", "Maillart", "Wy",
"Icarius", "Hermenion", "Gozelinus", "Talebot", "Grege",
"Strophius", "Aligern", "Arius", "Pain", "Lancelin",
"Guarin", "Euphratas", "Mecistes", "Astorge", "Mulius",
"Gorgion", "Bauto", "Oddo", "Aldo", "Alois",
"Faremanne", "Uwe", "Armentarius", "Eioneus", "Gilpin",
"Alysandir", "Fulbert", "Rollin", "Judo", "Epaphos",
"Erruki", "Amythaon", "Garibald", "Jacobo", "Kristos",
"Bangin", "Inautzi", "Bevon", "Agid", "Ambe",
"Kahumanu", "Ferant", "Aldo", "Thankmar", "Jurdan",
"Herewart", "Ifebrand", "Cosmas", "Willehelm", "Anytos",
"Hustaz", "Ivvanus", "Saer", "Androbulos", "Geffe",
"Antipatris", "Alizaunder", "Aistan", "Rogier", "Stichius",
"Eutharic", "Archelaos", "Girard", "Ngakaukawa", "Hue",
"Barrett", "Marko", "Aesculapius", "Paulin", "Baldric",
"Olympiodorus", "Alots", "Wenert", "Doolin", "Hluodohari",
"Baldemarus", "Lampo", "Pylas", "Tancard", "Otsoa",
"Sabas", "Hilderith", "Ludoldus", "Leuuine", "Lukos", "Leuvibild",
"Kallikrates", "Hernais", "Arnold", "Brygos",
"Gunthigis", "Landri", "Eutychianus", "Laurin", "Gilberd",
"Enetz", "Thidrek", "Digne", "Bisinus", "Betadur",
"Hippotas", "Konon", "Gratian", "Jal", "Elysandre", "Guiart",
"Charibert", "Waimar", "Morin", "Krokinos",
"Ambroise", "Aztore", "Teodric", "Pentheus", "Basilius", "Ilari",
"Melegros", "Hutch", "Onesiphorus", "Diggin",
"Saphrax", "Philotectes", "Herbelot", "Asopodoros", "Anshelmus",
"Turpin", "Chatbert", "Candac", "Phoenix", "Basilides",
"Felippe", "Tedbaldus", "Menandros", "Guernot", "Oseberne",
"Albin", "Fabien", "Hervey", "Kai", "Sirom",
"Liellus", "Hugline", "Wezilo", "Euthalius", "Taurin",
"Rannulf", "Hano", "Jodocus", "Fraunk", "Enops",
"Emond", "Isidor", "Theophylact", "Ham", "Gaillard",
"Deiotones", "Neozzo", "Amalaric", "Jacquelin", "Guimond",
"Birinus", "Garnotin", "Hank", "Vetranio", "Theodbald",
"Nikomedes", "Lovell", "Timote", "Pytheas", "Felippo",
"Hyakinthos", "Philoxenus", "Charopos", "Amal", "Hustaz",
"Marcoul", "Chaeremon", "Gundobad", "Algar", "Rollant",
"Benéoit", "Panares", "Raffo", "Rasequin", "Haiete", "Taureas",
"Soranus", "Edward", "Herbrand", "Speciosus",
"Athelardus", "Maugis", "Remon", "Trostheri", "Yvonnet",
"Otsoa", "Melampos", "Archimbalt", "Archytas", "Gebhard",
"Kenwrec", "Hamen", "Laodamas", "Hano", "Xenokrates",
"Chalin", "Pyrrhus", "Isidoros", "Boethus", "Quito",
"Ranulph", "Adalbert", "Kuonrat", "Benild", "Theobald",
"Igon", "Guerinnet", "Audemar", "Lizardi", "Geberic",
"Barthélemy", "Onomastos", "Dod", "Zenicetes", "Medus",
"Teodbald", "Agathocles", "Baudet", "Alwyne", "Aristides",
"Gorgion", "Chryses", "Hennsmann", "Juaneizu", "Geffrei",
"Herewart", "Ancus", "Baza", "Palamas", "Pagen",
"Charlet", "Philkin", "Gilow", "Tharuaro", "Remonnet", "Mamme",
"Dionysophanes", "Renart", "Baiardo", "Donestan",
"Menke", "Berengier", "Polyctor", "Hugh", "Merigot", "Aschetil",
"Garaona", "Diodorus", "Fromony", "Echepolus",
"Gawyn", "Marsilion", "Polybius", "Jatsu", "Adeluin", "Kenewrec",
"Helmhart", "Aeropus", "Kupe", "Gebun",
"Herodion", "Baalhaan", "Aeluuin", "Guiart", "Gemmel",
"Antiphales", "Cyprian", "Bawden", "Lehoi", "Harpagos",
"Blancandrin", "Chremes", "Digne", "Kritoboulos", "Hyperenor",
"Wandregisel", "Ernisius", "Willcock", "Umphrey", "Gundoald",
"Martel", "Ludewicus", "Edun", "Palladius", "Pandion", "Jollivet",
"Gerrart", "Harde", "Alkamenos", "Anchitel",
"Lanfranc", "Galafe", "Phythian", "Kepa", "Protogenes", "Rafold",
"Nob", "Hamnet", "Anaut", "Korax",
"Josclyn", "Hickie", "Cerularius", "Denysot", "Naimes",
"Beila", "Gaiseric", "Arsenios", "Rizon", "Phelipot",
"Larance", "Hecataeus", "Wintherus", "Flori", "Godefroi",
"Panares", "Ponteus", "Alysandir", "Rollin", "Ailmer",
"Joannes", "Woco", "Sinon", "Giro", "Hluodowig", "Gaitzka",
"Sjamme", "Demonax", "Ernst", "Itheus",
"Briarus", "Ippollitt", "Henko", "Harmatidas", "Ionnes",
"Poincet", "Onesiphorus", "Chrisogon", "Leuvibild", "Briarus",
"Gamelinus", "Wocco", "Chrysogonus", "Adalbrecht", "Osketel",
"Tettrino", "Gaiseric", "Rich", "Tisias", "Ibba",
"Remi", "Izotz", "Inachus", "Hipponicus", "Amphion", "Poincet",
"Arbitio", "Andhari", "Axular", "Ortzi",
"Orges", "Erchebald", "Hann", "Baldewyne", "Medus", "Grege",
"Tadica", "Widigast", "Adrastos", "Segarus",
"Wulurich", "Chalcodon", "Donat", "Berengerus", "Acrisius",
"Marmaduc", "Reipert", "Vidimir", "Guillemot", "Laodamas",
"Agamemnon", "Liulf", "Borani", "Atze", "Doneuuald", "Hydatius",
"Binizo", "Fardulf", "Payn", "Oroites",
"Firmo", "Aithanarid", "Strachys", "Humphery", "Rudesind",
"Bendis", "Huon", "Casjen", "Stallo", "Tyon",
"Percivale", "Albinus", "Bacenor", "Vivianus", "Ansila",
"Auguinare", "Ansehelm", "Gundehar", "Thankmar", "Seleukos",
"Beuca", "Eicko", "Genethlius", "Agapetus", "Cyriack", "Eucarpus",
"Adstan", "Scyllias", "Euthynos", "Phillipus",
"Wiebt", "Peirithous", "Meletius", "Poppe", "Perimedes", "Othon",
"Helmhart", "Raganald", "Ariaric", "Methodius"
};

// ======================= INTERNALLY DEFINED FUNCTIONS ======================

MERCHANT * get_closest_merchant(E_PLAYER e_player);
LISTING * get_merchant_listing_item_by_index(MERCHANT * merchant, unsigned int listing_index);
LISTING * search_merchant_listing_by_ID(MERCHANT * merchant, ITEM_IDS item_id);
LISTING * get_merchant_first_empty_listing(MERCHANT * merchant);

// ======================= EXTERNALLY DEFINED FUNCTIONS ======================

int double_buffer(void **, unsigned int *, unsigned int);
void init_enemy(E_ENEMY *, ivec2, vec2);
