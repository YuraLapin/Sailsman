#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <sstream>

using namespace std;
using namespace sf;

//Радиус и диаметр всех вершин
const int R = 50;
const int D = R * 2;

//Количество вершин графа из варианта 11 лабораторной работы
const int N = 7;

//Шрифт
const string FONT = "Arial.ttf";

//Размеры кнопок
const int btn_width = 115;
const int btn_height = 25;

//Вывод маршрута, записанного в вектор
ostream& operator<<(ostream& stream, const vector<string>& to_print)
{
	if (to_print.size() > 0)
	{
		for (int i = 0; i < to_print.size(); ++i)
		{
			stream << to_print[i];
			if (i != to_print.size() - 1)
			{
				stream << "-";
			}
		}
	}
	else
	{
		stream << "-";
	}
	
	stream << endl;
	return stream;
}

//Вывод матрицы
void print(const vector<vector<int>>& matrix, const vector<string> names)
{
	int size = names.size();

	for (int i = 0; i < size; ++i)
	{
		cout << i << ": " << names[i] << endl;
	}

	cout << endl << "-  ";

	for (int i = 0; i < size; ++i)
	{
		cout << i << "   ";
	}

	cout << endl;

	for (int i = 0; i < size; ++i)
	{
		cout << i << " ";

		for (int j = 0; j < size; ++j)
		{
			if (matrix[i][j] >= 0 && matrix[i][j] <= 9)
			{
				cout << " " << matrix[i][j] << "  ";
			}
			else
			{
				cout << " " << matrix[i][j] << " ";
			}
		}

		cout << endl;
	}
}

//Проверяет, есть ли элемент в векторе
bool check_insertion(const vector<string>& vector, const string& element)
{
	for (int i = 0; i < vector.size(); ++i)
	{
		if (vector[i] == element)
		{
			return true;
		}
	}
	return false;
}

struct Coords
{
	double x = 0;
	double y = 0;

	void set(const double& x, const double& y)
	{
		this->x = x;
		this->y = y;
	}
};

//Отрисовка вершины по заданным координатам. Если вершина не посещена - отрисует красную, иначе - зеленую
void draw_vertex(const Coords& coords, const sf::String& txt, const bool& visited, RenderWindow& window)
{
	CircleShape circle(R);
	circle.setOrigin(R, R);
	circle.setPosition(coords.x, coords.y);
	circle.setOutlineColor(Color::Black);
	circle.setOutlineThickness(3);
	if (visited == true)
	{
		circle.setFillColor(Color::Green);
	}
	else
	{
		circle.setFillColor(Color::Red);
	}

	Font font;
	font.loadFromFile(FONT);

	Text text(txt, font, R / 3);
	text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2);
	text.setPosition(coords.x, coords.y);
	text.setFillColor(Color::Black);

	window.draw(circle);
	window.draw(text);
}

//Отрисовка линии по координатам двух концов, а также отрисовка посередине линии заданного текста
void draw_line(const Coords& coords1, const Coords& coords2, const sf::String& txt, RenderWindow& window)
{
	vector<Vertex> line =
	{
		Vertex(Vector2f(coords1.x, coords1.y), Color::Black),
		Vertex(Vector2f(coords2.x, coords2.y), Color::Black)
	};

	Font font;
	font.loadFromFile(FONT);

	Text text(txt, font, R / 3);
	text.setPosition((coords1.x + coords2.x) / 2, (coords1.y + coords2.y) / 2);
	text.setOutlineThickness(5);
	text.setOutlineColor(Color::White);
	text.setFillColor(Color::Black);

	window.draw(&line[0], line.size(), Lines);
	window.draw(text);
}

//Отрисовка кнопки по координатам и по тексту
void draw_button(const Coords& coords, const int& width, const int& height, const string& txt, RenderWindow& window)
{
	RectangleShape button(Vector2f(width, height));
	button.setOrigin(width / 2, height / 2);
	button.setPosition(coords.x, coords.y);
	button.setFillColor(Color::Red);
	button.setOutlineThickness(3);
	button.setOutlineColor(Color::Black);

	Font font;
	font.loadFromFile(FONT);

	Text text(txt, font, R / 3);
	text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2);
	text.setPosition(coords.x, coords.y);
	text.setFillColor(Color::Black);

	window.draw(button);
	window.draw(text);
}

//Отрисовка сообщение по координатам и тексту
void draw_message(const Coords& coords, const string& txt, bool centered, RenderWindow& window)
{
	Font font;
	font.loadFromFile(FONT);

	Text message(txt, font, R / 3);
	if (centered == true)
	{
		message.setOrigin(message.getLocalBounds().width / 2, message.getLocalBounds().height / 2);
	}
	message.setPosition(coords.x, coords.y);
	message.setFillColor(Color::Black);

	window.draw(message);
}

class Graph
{
public:
	vector<string> vertex_list;
	vector<vector<int>> matrix;

	Graph()
	{

	}

	Graph(const Graph& another_one)
	{
		vertex_list = another_one.vertex_list;
		matrix = another_one.matrix;
	}

	Graph(const int& size)
	{
		matrix = vector<vector<int>>(size, vector<int>(size));

		for (int i = 0; i < size; ++i)
		{
			for (int j = 0; j < size; ++j)
			{
				matrix[i][j] = 0;
			}
		}
	}	

	int size()
	{
		return vertex_list.size();
	}	

	//Возвращает индекс вершины (или -1, если вершины нет в графе)
	int get_index(const string& vertex)
	{
		for (int i = 0; i < size(); ++i)
		{
			if (vertex_list[i] == vertex)
			{
				return i;
			}
		}
		return -1;
	}

	//Возвращает длину пути между двумя заданными вершинами (или 0, если хотя бы одной из вершин нет в графе)
	int get_weight(const string& vertex1, const string& vertex2)
	{
		int pos1 = get_index(vertex1);
		int pos2 = get_index(vertex2);

		if (pos1 == -1 || pos2 == -1)
		{
			cout << "Как минимум одна из вершин отсутствует в графе" << " " << vertex1 << " " << vertex2 << endl;
			return 0;
		}

		return matrix[pos1][pos2];
	}

	//Возвращает вектор с указателями на соседей заданной вершины
	vector<string> get_neighbour_list(const string& vertex)
	{
		vector<string> neighbour_list;
		int pos = get_index(vertex);

		if (pos != -1)
		{
			for (int i = 0; i < size(); ++i)
			{
				if (matrix[pos][i] > 0)
				{
					neighbour_list.push_back(vertex_list[i]);
				}
			}
		}

		return neighbour_list;
	}

	//Рекурсивное решение задачи коммивояжера
	vector<string> recursive_sailsman(vector<string> visited, string finish)
	{
		//Все возможные пути
		vector<vector<string>> options;

		//Кратчайший путь и его длина
		vector<string> final_route;
		int shortest = -1;

		//Промежуточный путь для рекурсивых повторений функции
		vector<string> da_way;

		//Отмечаем посещенную вершину
		visited.push_back(finish);

		//Выполняется, если посещены еще не все вершины
		if (visited.size() < vertex_list.size())
		{
			for (int i = 0; i < vertex_list.size(); ++i)
			{
				//Если вершины нет среди посещенных и к ней можно пройти, то дальше посещаем ее
				if (check_insertion(visited, vertex_list[i]) == false && get_weight(vertex_list[i], finish) > 0)
				{
					options.push_back(recursive_sailsman(visited, vertex_list[i]));
				}
			}

			//Если есть несколько путей, то выбираем кратчайший
			if (options.size() > 0)
			{
				for (int i = 0; i < options.size(); ++i)
				{
					if (options[i].size() > 0)
					{
						if (((get_route_distance(options[i]) + get_weight(options[i][0], finish)) < shortest) || shortest == -1)
						{
							shortest = get_route_distance(options[i]) + get_weight(options[i][0], finish);
							final_route = options[i];
						}
					}
				}

				//Если нашли какой-то путь, то добавим его к возвращаемому
				if (final_route.size() > 0)
				{
					da_way.push_back(finish);

					for (int i = 0; i < final_route.size(); ++i)
					{
						da_way.push_back(final_route[i]);
					}
				}
			}

			return da_way;
		}
		//Если посещены все вершины, то возвращаем итоговый маршрут
		else
		{
			//Если из конечной точки нет прямого пути в точку начала, то вернем пустой вектор
			if (get_weight(vertex_list[0], finish) > 0)
			{
				final_route.push_back(finish);
				final_route.push_back(vertex_list[0]);
			}

			return final_route;
		}
	}

	//Перестройка маршрута с учетом стартовой вершины
	void set_start(vector<string>& route, const string& start)
	{		
		if (route.size() > 0)
		{
			if (count(route.begin(), route.end(), start) > 0)
			{
				if (route[0] != start)
				{
					vector<string> ans;
					int start_i = 0;
					bool flag_break = false;
					for (int i = 0; i < route.size() - 1 && flag_break == false; ++i)
					{
						if (route[i] == start)
						{
							flag_break = true;
							start_i = i;
						}
					}
					for (int i = start_i; i < route.size() - 1; ++i)
					{
						ans.push_back(route[i]);
					}
					for (int i = 0; i < start_i; ++i)
					{
						ans.push_back(route[i]);
					}
					ans.push_back(start);
					route = ans;
				}
			}			
		}
	}

	//Постройка маршрута методом рекурсии
	vector<string> build_route(const string& start)
	{
		vector<string> visited;
		vector<string> ans = recursive_sailsman(visited, vertex_list[0]);
		set_start(ans, start);
		cout << "---------------------------------------------------------" << endl;
		print(matrix, vertex_list);
		cout << endl;
		cout << "Route length: " << get_route_distance(ans) << endl;
		cout << "Our route: " << ans << endl;
		return ans;
	}

	//Удаляет вершину и перестраивает маршрут
	vector<string> delete_vertex(const string& vertex, vector<Coords>& coords, const string& start)
	{
		int i = get_index(vertex);
		vertex_list.erase(vertex_list.begin() + i);
		matrix.erase(matrix.begin() + i);
		for (int j = 0; j < matrix.size(); ++j)
		{
			matrix[j].erase(matrix[j].begin() + i);
		}
		coords.erase(coords.begin() + i);

		return build_route(start);
	}

	void insert_vertex(const string& vertex)
	{
		vertex_list.push_back(vertex);
	}	

	void insert_edge(const string& vrtx1, const string& vrtx2, const int& weight = 1)
	{
		int pos1 = get_index(vrtx1);
		int pos2 = get_index(vrtx2);

		if (pos1 == -1 || pos2 == -1)
		{
			cout << "Как минимум одна из вершин отсутствует в графе" << endl;
		}
		else
		{
			if (matrix[pos1][pos2] == 0 and matrix[pos2][pos1] == 0)
			{
				matrix[pos1][pos2] = weight;
				matrix[pos2][pos1] = weight;
				matrix[pos1][pos1] = 0;
				matrix[pos2][pos2] = 0;
			}
			else
			{
				cout << "Ребро уже существует" << endl;
			}
		}
	}

	//Автоматическая генерация графа из варианта 11 лабораторной работы
	void autogen()
	{
		insert_vertex("Lisva");
		insert_vertex("Tinda");
		insert_vertex("Moscow");
		insert_vertex("Kazan");
		insert_vertex("Perm");
		insert_vertex("Ufa");
		insert_vertex("Orda");

		insert_edge("Moscow", "Ufa", 5);
		insert_edge("Ufa", "Orda", 19);
		insert_edge("Orda", "Tinda", 23);
		insert_edge("Tinda", "Kazan", 17);
		insert_edge("Lisva", "Kazan", 26);
		insert_edge("Perm", "Lisva", 9);
		insert_edge("Perm", "Moscow", 13);
		insert_edge("Perm", "Orda", 30);
		insert_edge("Ufa", "Kazan", 42);
	}	

	//Возвращает длину маршрута, записанного в вектор
	int get_route_distance(const vector<string>& route)
	{
		int dist = 0;
		for (int i = route.size() - 1; i > 0; --i)
		{
			dist += get_weight(route[i - 1], route[i]);
		}
		return dist;
	}	

	//Визуальный вывод через библиотеку SFML
	void draw(vector<string>& route)
	{
		int width = 1000;
		int height = 950;
		RenderWindow MainWindow(VideoMode(width, height), "Our Graph", sf::Style::Titlebar | sf::Style::Close);

		//Счетчик нажатий на кнопку Next step
		int count = 0;

		//Координаты всех вершин графа
		vector<Coords> coords(size());
		coords[0].set(3 * D, 6 * D);
		coords[1].set(8 * D, 5 * D);
		coords[2].set(D, D);
		coords[3].set(6 * D, 8 * D);
		coords[4].set(2 * D, 3 * D);
		coords[5].set(4 * D, D);
		coords[6].set(7 * D, 2 * D);		

		//Истина, если какую-то вершину нужно двигать
		bool flag_move = false;		

		//Индекс вершины, которую нужно сдвинуть
		int to_move_index;

		//Разница между координатами курсора и координатами вершины, которой он касается
		int dx, dy;

		//Стартовая точка маршрута
		string start = "Tinda";

		while (MainWindow.isOpen())
		{
			Event event;

			while (MainWindow.pollEvent(event))
			{
				//Событие закрытия окна
				if (event.type == Event::Closed)
				{
					MainWindow.close();					
				}

				else
				//Событие нажатия на какую-то из кнопок мыши
				if (event.type == Event::MouseButtonPressed)
				{
					//Событие нажатия на левую кнопку мыши
					if (event.mouseButton.button == Mouse::Left)
					{						
						//Событие нажатия на кнопку Next step
						if (Mouse::getPosition(MainWindow).x > 100 - btn_width / 2 &&
							Mouse::getPosition(MainWindow).x < 100 + btn_width / 2 &&
							Mouse::getPosition(MainWindow).y > 875 - btn_height / 2 &&
							Mouse::getPosition(MainWindow).y < 875 + btn_height / 2)
						{
							if (count < size())
							{
								++count;
							}
						}					
						
						else
						//Событие нажатия на кнопку Add Vertex
						if (Mouse::getPosition(MainWindow).x > 100 - btn_width / 2 &&
							Mouse::getPosition(MainWindow).x < 100 + btn_width / 2 &&
							Mouse::getPosition(MainWindow).y > 835 - btn_height / 2 &&
							Mouse::getPosition(MainWindow).y < 835 + btn_height / 2)
						{
							//Сбросим количество отрисованных шагов маршрута (потому что машрут после добавления будет перестроен)
							count = 0;
							
							cout << "Введите название города: ";
							string name1;
							cin >> name1;

							//Защита от повторного ввода вершины
							while (get_index(name1) != -1)
							{
								cout << "Город уже присутствует в графе" << endl;
								cout << "Введите название города: ";								
								cin >> name1;
							}			
															
							insert_vertex(name1);									

							//Добавим строку в матрицу
							for (int j = 0; j < vertex_list.size() - 1; ++j)
							{
								matrix[j].push_back(0);
							}

							//Добавим колонну в матрицу
							vector<int> additional_column(vertex_list.size());
							for (int j = 0; j < additional_column.size(); ++j)
							{
								additional_column[j] = 0;
							}
							matrix.push_back(additional_column);
								
							cout << "Введите количество городов, которые соединены с этой вершиной: ";
							int count;
							cin >> count;

							//Защита от ввода неправильного количества городов
							while (count < 0)
							{
								cout << "Количество городов не может быть отрицательной величиной" << endl;
								cout << "Введите количество городов, которые соединены с этой вершиной: ";
								cin >> count;
							}

							for (int i = 0; i < count; ++i)
							{									
								cout << "Введите название города: ";
								string name2;
								cin >> name2;

								//Защита от ввода несуществующего города
								while (get_index(name2) == -1)
								{
									cout << "Город отсутствует в графе" << endl;
									cout << "Введите название города: ";									
									cin >> name2;
								}

								//Защита от повтора имеющейся связи
								while (get_weight(name1, name2) > 0)
								{
									cout << "Связь между городами уже существует" << endl;
									cout << "Введите название города: ";									
									cin >> name2;
								}

								cout << "Введите расстояние: ";
								int dist;
								cin >> dist;

								//Защита от ввода неположительного расстояния
								while (dist <= 0)
								{
									cout << "Расстояние должно быть положительной величиной" << endl;
									cout << "Введите расстояние: ";									
									cin >> name2;
								}

								insert_edge(name1, name2, dist);
							}						

							//Перестроим маршрут и выведем его длину						
							route = build_route(start);

							//Вынесем новый город в центр экрана, чтобы пользователь сам мог подвинуть его
							Coords new_one;
							new_one.set(width / 2, height / 2);
							coords.push_back(new_one);
							
						}						

						//Событие перетаскивания вершины
						else
						{
							for (int i = 0; i < size(); ++i)
							{
								if (Mouse::getPosition(MainWindow).x > coords[i].x - R &&
									Mouse::getPosition(MainWindow).x < coords[i].x + R &&
									Mouse::getPosition(MainWindow).y > coords[i].y - R &&
									Mouse::getPosition(MainWindow).y < coords[i].y + R)
								{
									//Запоминаем, какую вершину нужно двигать
									to_move_index = i;

									//Запоминаем координаты вершины относительно курсора
									dx = event.mouseButton.x - coords[i].x;
									dy = event.mouseButton.y - coords[i].y;

									//Запоминаем, что какая-то из вершин дожна следовать за курсором
									flag_move = true;
								}
							}
						}						
					}

					else
					//Событие нажатия на правую кнопку мыши для удаления вершины
					if (event.mouseButton.button == Mouse::Right)
					{
						bool flag_break = false;
						for (int i = 0; i < size() && flag_break == false; ++i)
						{
							if (Mouse::getPosition(MainWindow).x > coords[i].x - R &&
								Mouse::getPosition(MainWindow).x < coords[i].x + R &&
								Mouse::getPosition(MainWindow).y > coords[i].y - R &&
								Mouse::getPosition(MainWindow).y < coords[i].y + R)
							{		
								//Сброс количества отрисованных пунктов маршрута, ведь маршрут будет перестроен
								count = 0;

								if (start == vertex_list[i])
								{
									start = vertex_list[0];
								}
								route = delete_vertex(vertex_list[i], coords, start);								
								flag_break = true;
							}
						}
					}

					else
					//Событие нажатия на среднюю кнопку для задания стартовой точки
					if (event.mouseButton.button == Mouse::Middle)
					{
						bool flag_break = false;
						for (int i = 0; i < size() && flag_break == false; ++i)
						{
							if (Mouse::getPosition(MainWindow).x > coords[i].x - R &&
								Mouse::getPosition(MainWindow).x < coords[i].x + R &&
								Mouse::getPosition(MainWindow).y > coords[i].y - R &&
								Mouse::getPosition(MainWindow).y < coords[i].y + R)
							{
								//Сброс количества отрисованных пунктов маршрута, ведь маршрут будет перестроен
								count = 0;
								
								start = vertex_list[i];
								route = build_route(start);
								flag_break = true;
							}
						}
					}
				}

				else
				//Событие отпускания левой кнопки мыши
				if (event.type == Event::MouseButtonReleased)
				{
					if (event.key.code == Mouse::Left)
					{
						flag_move = false;
					}					
				}				
			}

			MainWindow.clear(Color::White);

			//Если зажата левая кнопка мыши, то двигает координаты вершины, на которой находится курсор (если такая есть)
			if (flag_move == true)
			{
				//Горизонтальная проверка выхода за границы
				if (((Mouse::getPosition(MainWindow).x - dx) > R) && ((Mouse::getPosition(MainWindow).x - dx) < (width - R)))
				{
					coords[to_move_index].set(Mouse::getPosition(MainWindow).x - dx, coords[to_move_index].y);
				}

				//Вертикальная проверка выхода за границы
				if (((Mouse::getPosition(MainWindow).y - dy) > R) && ((Mouse::getPosition(MainWindow).y - dy) < (height - R)))
				{
					coords[to_move_index].set(coords[to_move_index].x, Mouse::getPosition(MainWindow).y - dy);
				}
				
			}

			//Отрисовка линий, соединяющих вершины, а также длин путей между ними
			for (int i = 0; i < size(); ++i)
			{
				vector<string> neighbours = get_neighbour_list(vertex_list[i]);
				for (int j = 0; j < neighbours.size(); ++j)
				{
					stringstream txt;
					txt << matrix[i][get_index(neighbours[j])];
					draw_line(coords[i], coords[get_index(neighbours[j])], txt.str(), MainWindow);
				}
			}

			//Отрисовка всех вершин красным цветом
			for (int i = 0; i < size(); ++i)
			{
				stringstream i_txt;
				i_txt << vertex_list[i];
				draw_vertex(coords[i], i_txt.str(), false, MainWindow);
			}

			//Поверх рисуется count зеленых вершин - первые count остановок нашего маршрута
			for (int i = 0; i < count; ++i)
			{
				if (route.size() >= count)
				{
					draw_vertex(coords[get_index(route[i])], route[i], true, MainWindow);
				}
			}

			//Координаты кнопок и их текст
			Coords button_coords;
			string text;

			//Отрисовка кнопки Next step			
			button_coords.set(100, 875);
			text = "Next Step";
			draw_button(button_coords, btn_width, btn_height, text, MainWindow);

			//Отрисовка кнопки Add Vertex			
			button_coords.set(100, 835);
			text = "Add Vertex";
			draw_button(button_coords, btn_width, btn_height, text, MainWindow);

			//Координаты текста
			Coords text_coords;
			
			//Сообщение об управлении перетаскиванием вершин
			text_coords.set(40, 680);
			text = "Left Mouse Button to move vertex";
			draw_message(text_coords, text, false, MainWindow);

			//Сообщение об управлении удалением
			text_coords.set(40, 700);
			text = "Right Mouse Button to delete vertex";
			draw_message(text_coords, text, false, MainWindow);

			//Сообщение об управлении выбора стартовой вершины
			text_coords.set(40, 720);
			text = "Middle Mouse Button to set starting vertex";
			draw_message(text_coords, text, false, MainWindow);

			//Сообщение о текущей стартовой вершине
			text_coords.set(40, 740);
			text = "Current starting vertex: " + start;
			draw_message(text_coords, text, false, MainWindow);

			//Переменная для записи текста, содержащего информацию о маршруте
			stringstream route_txt;

			//Сообщение о текущем маршруте
			text_coords.set(40, 760);
			route_txt << "Current route: " << route;			
			draw_message(text_coords, route_txt.str(), false, MainWindow);

			//Сообщение о текущей длине маршрута
			text_coords.set(40, 780);
			route_txt.str("");
			route_txt << "Current route length: " << get_route_distance(route);
			draw_message(text_coords, route_txt.str(), false, MainWindow);

			MainWindow.display();
		}
	}
};

int main()
{
	setlocale(LC_ALL, "RUS");

	//Создание графа из варианта 11 и вывод его матрицы
	Graph example(N);
	example.autogen();	

	//Постройка и вывод маршрута и его длины	
	vector<string> route = example.build_route("Tinda");	

	system("pause");

	//Визуальное представление графа и маршрута
	example.draw(route);	

	return 0;
}