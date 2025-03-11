// 프로젝트 설정의 Description 페이지에서 저작권 정보를 작성하세요.
#include "Game/EGameMode.h"
#include "EGameMode.h"
#include "Player/EPlayerController.h"

// AEGameMode 클래스의 생성자
AEGameMode::AEGameMode()
{
    // Pawn 클래스 지정:
    // 게임에서 플레이어가 조종할 기본 Pawn 클래스를 설정합니다.
    // ConstructorHelpers::FClassFinder를 사용하여 지정한 경로에서 Pawn 클래스를 찾습니다.
    static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassRef(TEXT("/Game/Eden/Blueprints/BP_CharacterPlayer.BP_CharacterPlayer_C"));
    if (DefaultPawnClassRef.Class)
    {
        // 지정한 경로에서 Pawn 클래스가 정상적으로 로드되었다면,
        // DefaultPawnClass에 해당 클래스를 할당하여 게임에서 기본 Pawn으로 사용합니다.
        DefaultPawnClass = DefaultPawnClassRef.Class;
    }

    // PlayerController 클래스 지정:
    // 플레이어의 입력과 카메라 제어 등 게임 로직을 담당할 PlayerController 클래스를 설정합니다.
    // ConstructorHelpers::FClassFinder를 사용하여 지정한 경로에서 PlayerController 클래스를 찾습니다.
    static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Game/Eden/Blueprints/BP_ECharacterController.BP_ECharacterController_C"));
    if (PlayerControllerClassRef.Class)
    {
        // 지정한 경로에서 PlayerController 클래스가 정상적으로 로드되었다면,
        // PlayerControllerClass에 해당 클래스를 할당하여 게임에서 기본 컨트롤러로 사용합니다.
        PlayerControllerClass = PlayerControllerClassRef.Class;
    }
}
