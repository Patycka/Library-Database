#include "stdafx.h"
#include "LibraryManager.h"



LibraryManager::LibraryManager()
{
}


LibraryManager::~LibraryManager()
{
}

Book LibraryManager::getBook(int bookId) {
	try {
		BooksRepository* booksRepository = new BooksRepository("books.xml");
		Book book = booksRepository->getById(bookId);
		delete(booksRepository);
		return book;
	}
	catch (runtime_error& e) {
		string innerException(e.what());
		throw runtime_error(EXCEPTION_COULD_NOT_GET_BOOK + innerException);
	}
}

LibraryUser LibraryManager::getUser(string userName) {
	try {
		UsersRepository* usersRepository = new UsersRepository("users.xml");

		LibraryUser user = usersRepository->getByName(userName);
		
		delete(usersRepository);

		return user;
	}
	catch (runtime_error& e) {
		string innerException(e.what());
		throw runtime_error(EXCEPTION_COULD_NOT_GET_USER + innerException);
	}
}

list<Book> LibraryManager::getBooks(string bookName) {
	try {
		BooksRepository* booksRepository = new BooksRepository("books.xml");
		
		list<Book> books = booksRepository->getByName(bookName);

		delete(booksRepository);

		return books;
	}
	catch (runtime_error& e) {
		string innerException(e.what());
		throw runtime_error(EXCEPTION_COULD_NOT_GET_BOOKS + innerException);
	}
}

Book LibraryManager::getFirstAvailableBook(string bookName) {
	try {
		list<Book> books = getBooks(bookName);
		for (list<Book>::iterator iterator = books.begin(), end = books.end(); iterator != end; ++iterator) {
			if (!(iterator->isBorrowed())) return *iterator;
		}
		throw runtime_error(EXCEPTION_NO_FREE_BOOKS_FOUND);
	}
	catch (runtime_error& e) {
		string innerException(e.what());
		throw runtime_error(EXCEPTION_COULD_NOT_GET_AVAILABLE_BOOK + innerException);
	}
}

void LibraryManager::addNewUserToLibrary(string userName) {
	try {
		UsersRepository* usersRepository = new UsersRepository("users.xml");

		LibraryUser newUser(userName);

		if (usersRepository->exists(newUser)) throw runtime_error(EXCEPTION_USER_EXISTS);

		usersRepository->add(newUser);

		delete(usersRepository);
	}
	catch (runtime_error& e) {
		string innerException(e.what());
		throw runtime_error(EXCEPTION_COULD_NOT_ADD_NEW_USER_TO_LIBRARY + innerException);
	}
}

void LibraryManager::removeUserFromLibrary(string userName, bool force) {
	try {
		UsersRepository* usersRepository = new UsersRepository("users.xml");

		LibraryUser user = usersRepository->getByName(userName);

		if (!user.hasAnyBooks()) usersRepository->remove(user);
		else if (force && user.hasAnyBooks()) {
			list<int> userBooks = user.getBooks();
			for (list<int>::iterator iterator = userBooks.begin(), end = userBooks.end(); iterator != end; ++iterator) {
				returnBookFromUser(*iterator, user.getName());
			}
		}
		else if (!force && user.hasAnyBooks()) throw runtime_error(EXCEPTION_USER_HAS_NON_RETURNED_BOOKS);

		delete(usersRepository);
	}
	catch (runtime_error& e) {
		string innerException(e.what());
		throw runtime_error(EXCEPTION_COULD_NOT_REMOVE_USER_FROM_LIBRARY + innerException);
	}
}

void LibraryManager::addNewBookToLibrary(string bookName) {
	try {
		BooksRepository* booksRepository = new BooksRepository("books.xml");

		Book newBook(bookName);

		if (booksRepository->exists(newBook)) throw runtime_error(EXCEPTION_BOOK_EXISTS);

		booksRepository->add(newBook);

		delete(booksRepository);
	}
	catch (runtime_error& e) {
		string innerException(e.what());
		throw runtime_error(EXCEPTION_COULD_NOT_ADD_NEW_BOOK_TO_LIBRARY + innerException);
	}
}

void LibraryManager::removeBookFromLibrary(int bookId, bool force) {
	try {
		BooksRepository* booksRepository = new BooksRepository("books.xml");
		UsersRepository* usersRepository = new UsersRepository("users.xml");

		Book book = booksRepository->getById(bookId);

		if (!book.isBorrowed()) booksRepository->remove(book);
		else if (force && book.isBorrowed()) {
			LibraryUser bookReader = usersRepository->getByName(book.getReader());
			bookReader.removeBook(book.getId());
			usersRepository->update(bookReader);
		}
		else if (!force && book.isBorrowed()) throw runtime_error(EXCEPTION_BOOK_IS_BORROWED);

		delete(booksRepository);
		delete(usersRepository);
	}
	catch (runtime_error& e) {
		string innerException(e.what());
		throw runtime_error(EXCEPTION_COULD_NOT_REMOVE_BOOK_FROM_LIBRARY + innerException);
	}
}

void LibraryManager::lendBookToUser(int bookId, string userName, string returnDate) {
	try {
		UsersRepository* usersRepository = new UsersRepository("users.xml");
		BooksRepository* booksRepository = new BooksRepository("books.xml");

		Book book = booksRepository->getById(bookId);
		LibraryUser user = usersRepository->getByName(userName);

		if (book.isBorrowed()) throw runtime_error(EXCEPTION_BOOK_IS_BORROWED);
		if (user.hasBook(bookId)) throw runtime_error(EXCEPTION_USER_HAS_BOOK);

		book.setReader(user.getName());
		book.setReturnDate(returnDate);
		user.addBook(bookId);

		booksRepository->update(book);
		usersRepository->update(user);

		delete(usersRepository);
		delete(booksRepository);
	}
	catch (runtime_error& e) {
		string innerException(e.what());
		throw runtime_error(EXCEPTION_COULD_NOT_LEND_BOOK_TO_USER + innerException);
	}
}

void LibraryManager::returnBookFromUser(int bookId, string userName) {
	try {
		UsersRepository* usersRepository = new UsersRepository("users.xml");
		BooksRepository* booksRepository = new BooksRepository("books.xml");

		Book book = booksRepository->getById(bookId);
		LibraryUser user = usersRepository->getByName(userName);

		if (!book.isBorrowed()) throw runtime_error(EXCEPTION_BOOK_IS_NOT_BORROWED);
		if (!user.hasBook(bookId)) throw runtime_error(EXCEPTION_USER_DOES_NOT_HAVE_BOOK);

		book.setReader("");
		book.setReturnDate("");
		user.removeBook(bookId);

		booksRepository->update(book);
		usersRepository->update(user);

		delete(usersRepository);
		delete(booksRepository);
	}
	catch (runtime_error& e) {
		string innerException(e.what());
		throw runtime_error(EXCEPTION_COULD_NOT_RETURN_BOOK_FROM_USER + innerException);
	}
}

list<Book> LibraryManager::getUserBooks(string userName) {
	try {
		UsersRepository* usersRepository = new UsersRepository("users.xml");
		BooksRepository* booksRepository = new BooksRepository("books.xml");

		LibraryUser user = usersRepository->getByName(userName);

		list<int> userBookIds = user.getBooks();

		list<Book> userBooks;
		for (list<int>::iterator iterator = userBookIds.begin(), end = userBookIds.end(); iterator != end; ++iterator) {
			Book currentBook = booksRepository->getById(*iterator);
			userBooks.push_back(currentBook);
		}

		delete(usersRepository);
		delete(booksRepository);

		return userBooks;
	}
	catch (runtime_error& e) {
		string innerException(e.what());
		throw runtime_error(EXCEPTION_COULD_NOT_GET_USER_BOOKS + innerException);
	}
}

LibraryUser LibraryManager::getCurrentBookUser(int bookId) {
	try {
		UsersRepository* usersRepository = new UsersRepository("users.xml");
		BooksRepository* booksRepository = new BooksRepository("books.xml");

		Book book = booksRepository->getById(bookId);

		if (book.isBorrowed()) {
			LibraryUser user = usersRepository->getByName(book.getReader());

			delete(usersRepository);
			delete(booksRepository);

			return user;
		}
		else {
			throw runtime_error(EXCEPTION_BOOK_IS_NOT_BORROWED);
		}
	}
	catch (runtime_error& e) {
		string innerException(e.what());
		throw runtime_error(EXCEPTION_COULD_NOT_GET_BOOK_USER + innerException);
	}
}

list<LibraryUser> LibraryManager::getAllUsers() {
	try {
		UsersRepository* usersRepository = new UsersRepository("users.xml");

		list<LibraryUser> allUsers = usersRepository->getAll();

		delete(usersRepository);

		return allUsers;
	}
	catch (runtime_error& e) {
		string innerException(e.what());
		throw runtime_error(EXCEPTION_COULD_NOT_GET_USERS + innerException);
	}
}

list<Book> LibraryManager::getAllBooks() {
	try {
		BooksRepository* booksRepository = new BooksRepository("books.xml");

		list<Book> allBooks = booksRepository->getAll();

		delete(booksRepository);

		return allBooks;
	}
	catch (runtime_error& e) {
		string innerException(e.what());
		throw runtime_error(EXCEPTION_COULD_NOT_GET_ALL_BOOKS + innerException);
	}
}




list<Book> LibraryManager::getAllBorrowedBooks() {
	try {
		BooksRepository* booksRepository = new BooksRepository("books.xml");

		list<Book> allBooks = booksRepository->getAll();

		list<Book> borrowedBooks;
		for (list<Book>::iterator iterator = allBooks.begin(), end = allBooks.end(); iterator != end; ++iterator) {
			if(iterator->isBorrowed()) borrowedBooks.push_back(*iterator);
		}

		delete(booksRepository);

		return borrowedBooks;
	}
	catch (runtime_error& e) {
		string innerException(e.what());
		throw runtime_error(EXCEPTION_COULD_NOT_GET_BORROWED_BOOKS + innerException);
	}
}

list<Book> LibraryManager::getAllExpiredBooks() {
	try {
		BooksRepository* booksRepository = new BooksRepository("books.xml");

		list<Book> allBooks = booksRepository->getAll();

		list<Book> expiredBooks;
		for (list<Book>::iterator iterator = allBooks.begin(), end = allBooks.end(); iterator != end; ++iterator) {
			if (iterator->isReturnTimeExpired()) expiredBooks.push_back(*iterator);
		}

		delete(booksRepository);

		return expiredBooks;
	}
	catch (runtime_error& e) {
		string innerException(e.what());
		throw runtime_error(EXCEPTION_COULD_NOT_GET_BORROWED_BOOKS + innerException);
	}
}

list<LibraryUser> LibraryManager::getAllUsersWithExpiredBooks() {
	try {
		UsersRepository* usersRepository = new UsersRepository("users.xml");

		list<Book> allExpiredBooks = getAllExpiredBooks();
		list<LibraryUser> usersWithExpiredBooks;
		for (list<Book>::iterator iterator = allExpiredBooks.begin(), end = allExpiredBooks.end(); iterator != end; ++iterator) {
			string bookReaderName = iterator->getReader();
			LibraryUser user = usersRepository->getByName(bookReaderName);
			usersWithExpiredBooks.push_back(user);
		}

		delete(usersRepository);

		return usersWithExpiredBooks;
	}
	catch (runtime_error& e) {
		string innerException(e.what());
		throw runtime_error(EXCEPTION_COULD_NOT_GET_USERS_WITH_EXPIRED_BOOKS + innerException);
	}
}