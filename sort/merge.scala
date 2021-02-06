
object main {
    def merge(l1: List[Int], l2: List[Int]) : List[Int] = {
        if(l1.isEmpty)
            l2
        else if(l2.isEmpty)
            l1
        else if(l1.head <= l2.head)
            l1.head :: merge(l1.tail, l2)
        else
            l2.head :: merge(l1, l2.tail)
    }

    def merge_sort(list: List[Int]) : List[Int] = {
        if(list.length == 1) {
            return list
        }
        val l1 = merge_sort(list.drop(list.size / 2))
        val l2 = merge_sort(list.dropRight(list.size / 2))
        merge(l1, l2)
    }
 
    def main(args: Array[String]): Unit = {
        print(merge_sort(List(3, 5, 3, 7, 5, 8, 32, 1, 2, 5, 3, 0)))
    }
}