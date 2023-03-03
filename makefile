lists_debug:
	gcc -g "./dataStructures/node/node.c" "./dataStructures/lists/linkedList.c" source.c -o source 
binary_tree_debug:
	gcc -g "./dataStructures/node/node.c" "./dataStructures/trees/binaryTree.c" binary_test.c -o binary_test
dictionary_tree_debug:
	gcc -g "./dataStructures/node/node.c" "./dataStructures/trees/binaryTree.c" "./dataStructures/dictionaries_tree/dictionary.c" dictionary_test.c -o dictionary_test
server_debug:
	gcc -g "./dataStructures/node/node.c" "./dataStructures/lists/linkedList.c" "./dataStructures/queues/queue.c" "./dataStructures/trees/binaryTree.c" "./dataStructures/dictionaries_tree/dictionary.c" "./networking/protocols/HTTPRequest.c" "./system/ThreadPool.c" "./networking/nodes/HTTPServer.c" "./networking/nodes/server.c"  server_test.c -o server_test -lpthread