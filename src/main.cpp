#include "Controller.h"

/**
 * @brief Главная функция приложения.
 *
 * Создаёт и запускает контроллер приложения реестра студентов.
 *
 * @return 0 при успешном завершении
 */
int main()
{
    university::Controller app;
    app.run();
    return 0;
}