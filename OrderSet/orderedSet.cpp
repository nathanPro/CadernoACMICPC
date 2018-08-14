#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#define GNU __gnu_pbds

namespace GNU { typedef tree<int, null_type, less<int>, rb_tree_tag,
	tree_order_statistics_node_update> ordered_set; }
using GNU::ordered_set;
// null_type pode mudar pra virar map
// multiset e multimap nao tem suporte aqui, tem que sair usando pair pelo jeito
// less eh o comparador

ordered_set s;

// s.insert(int) s.erase(iterator ou int)
// end(s) == s.find_by_order(int) -> s nao contem int
// find_by_order retorna o iterator pro kth maior elemento (comecando do 0)
// 	(int) *s.find_by_order(int)
// order_of_key retorna o numero de elementos no set estritamente menor que int
// 	(int) s.order_of_key(int)
