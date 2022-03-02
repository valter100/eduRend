
#include "Scene.h"

Scene::Scene(
	ID3D11Device* dxdevice,
	ID3D11DeviceContext* dxdevice_context,
	int window_width,
	int window_height) :
	dxdevice(dxdevice),
	dxdevice_context(dxdevice_context),
	window_width(window_width),
	window_height(window_height)
{ }

void Scene::WindowResize(
	int window_width,
	int window_height)
{
	this->window_width = window_width;
	this->window_height = window_height;
}

OurTestScene::OurTestScene(
	ID3D11Device* dxdevice,
	ID3D11DeviceContext* dxdevice_context,
	int window_width,
	int window_height) :
	Scene(dxdevice, dxdevice_context, window_width, window_height)
{
	source1 = new Light(vec3f(0, -0.0f, -20), true, dxdevice, dxdevice_context);
	source1->AddAnimationPath(vec3f(6, -0.0f, -15.0f));
	source1->AddAnimationPath(vec3f(8, -0.0f, -8.0f));
	source1->AddAnimationPath(vec3f(6, -0.0f, -1.0f));
	source1->AddAnimationPath(vec3f(0, -0.0f, 2));
	source1->AddAnimationPath(vec3f(-6, -0.0f, -1.0f));
	source1->AddAnimationPath(vec3f(-8, -0.0f, -8.0f));
	source1->AddAnimationPath(vec3f(-6, -0.0f, -15.0f));

	source2 = new Light(vec3f(0, 20, -20), true, dxdevice, dxdevice_context);
	source2->AddAnimationPath(vec3f(-10, -2, -20));
	source2->AddAnimationPath(vec3f(10, -2, -20));

	InitTransformationBuffer();
	// + init other CBuffers
	InitCameraAndLightBuffer();
	CreateSamplerState();
}

//
// Called once at initialization
//
void OurTestScene::Init()
{
	camera = new Camera(
		45.0f * fTO_RAD,		// field-of-view (radians)
		(float)window_width / window_height,	// aspect ratio
		0.5f,					// z-near plane (everything closer will be clipped/removed)
		500.0f);				// z-far plane (everything further will be clipped/removed)

	// Move camera to (0,0,5)
	camera->moveTo({ 0, 0, 5 });
	lightSources[0] = source1;
	lightSources[1] = source2;

	cube = new Cube(dxdevice, dxdevice_context, nullptr);
	secondCube = new Cube(dxdevice, dxdevice_context, cube);
	thirdCube = new Cube(dxdevice, dxdevice_context, secondCube);

	fourthCube = new Cube(dxdevice, dxdevice_context, cube);
	fifthCube = new Cube(dxdevice, dxdevice_context, fourthCube);
	sixthCube = new Cube(dxdevice, dxdevice_context, fifthCube);

	cubes.push_back(cube); models.push_back(cube);
	cubes.push_back(secondCube); models.push_back(secondCube);
	cubes.push_back(thirdCube); models.push_back(thirdCube);
	cubes.push_back(fourthCube); models.push_back(fourthCube);
	cubes.push_back(fifthCube); models.push_back(fifthCube);
	cubes.push_back(sixthCube); models.push_back(sixthCube);

	sponza = new OBJModel("assets/crytek-sponza/sponza.obj", dxdevice, dxdevice_context, false);
	models.push_back(sponza);
	bush = new OBJModel("assets/objects/bush/bush.obj", dxdevice, dxdevice_context, false);
	models.push_back(bush);
	character = new OBJModel("assets/objects/character/character.obj", dxdevice, dxdevice_context, false);
	models.push_back(character);
	train = new OBJModel("assets/objects/train/train.obj", dxdevice, dxdevice_context, false);
	models.push_back(train);
	gun = new Gun("assets/objects/gun/gun.obj", dxdevice, dxdevice_context, camera, 0.5f);
	models.push_back(gun);
	sphere = new OBJModel("assets/objects/sphere/sphere.obj", dxdevice, dxdevice_context, false);
	models.push_back(sphere);
	skybox = new OBJModel("assets/objects/skybox/skybox.obj", dxdevice, dxdevice_context, true);
	models.push_back(skybox);
	ToggleClipping(true);

	const char* filePaths[6] =
	{
		"assets/cubemaps/debug_cubemap/debug_posx.png",
		"assets/cubemaps/debug_cubemap/debug_negx.png",
		"assets/cubemaps/debug_cubemap/debug_posy.png",
		"assets/cubemaps/debug_cubemap/debug_negy.png",
		"assets/cubemaps/debug_cubemap/debug_posz.png",
		"assets/cubemaps/debug_cubemap/debug_negz.png"
	};

	HRESULT hr = LoadCubeTextureFromFile(dxdevice,
		filePaths,
		&cubeTexture);

	std::cout << "loaded cubemap: " << cubeTexture << (SUCCEEDED(hr) ? " - OK" : "- FAILED") << std::endl;
}

//
// Called every frame
// dt (seconds) is time elapsed since the previous frame
//
void OurTestScene::Update(float dt, InputHandler* input_handler)
{
	gun->Update();

	for (Light* light : lightSources)
	{
		if (light->IsAnimated())
		{
			light->Update();
		}
	}

#pragma region Camera Controls

	if (input_handler->IsKeyPressed(Keys::Right) && !isPressingKey)
	{
		samplerStateIndex += 1;

		if (samplerStateIndex >= samplerStates.size())
		{
			samplerStateIndex = 0;
		}

		std::cout << samplerStateIndex << std::endl;
		isPressingKey = true;
	}
	else if (input_handler->IsKeyPressed(Keys::Left) && !isPressingKey)
	{
		samplerStateIndex -= 1;

		if (samplerStateIndex < 0)
		{
			samplerStateIndex = samplerStates.size() - 1;
		}

		std::cout << samplerStateIndex << std::endl;
		isPressingKey = true;
	}

	if (input_handler->IsKeyPressed(Keys::Space) && !isPressingKey)
	{
		camera->SaveView();
		isPressingKey = true;
	}
	if (input_handler->IsKeyPressed(Keys::Esc) && !isPressingKey)
	{
		ToggleClipping(cursorIsShowing);
		isPressingKey = true;
	}

	if ((input_handler->IsKeyPressed(Keys::Up)))
		camera->ChangeFoV("in");
	if (input_handler->IsKeyPressed(Keys::Down))
		camera->ChangeFoV("out");

	if (!cursorIsShowing)
	{
		if (input_handler->IsKeyPressed(Keys::W))
			camera->move({ 0.0f, 0.0f, -camera_vel * dt });
		if (input_handler->IsKeyPressed(Keys::S))
			camera->move({ 0.0f, 0.0f, camera_vel * dt });
		if (input_handler->IsKeyPressed(Keys::D))
			camera->move({ camera_vel * dt, 0.0f, 0.0f });
		if (input_handler->IsKeyPressed(Keys::A))
			camera->move({ -camera_vel * dt, 0.0f, 0.0f });

		camera->Rotate(input_handler->GetMouseDeltaX(), input_handler->GetMouseDeltaY());
	}

	if ((input_handler->IsLeftMouseButtonPressed()))
	{
		if (cursorIsShowing)
		{
			ToggleClipping(true);
			ShowCursor(false);
		}

		if (gun->CanShoot())
		{
			gun->Shoot();
		}
	}


	if (!input_handler->IsKeyPressed(Keys::Right) &&
		!input_handler->IsKeyPressed(Keys::Left) &&
		!input_handler->IsKeyPressed(Keys::Space) &&
		!input_handler->IsKeyPressed(Keys::Esc))
	{
		isPressingKey = false;
	}

#pragma endregion
#pragma region Cubes

	//Independent scaling
	cube->SetTransformMatrix(
		mat4f::translation(-5, -3, -7),
		mat4f::rotation(angle * 0, 0.0f, 1.0f, 0.0f),
		mat4f::scaling(1.0f, 1.0f, 1.0f));

	secondCube->SetTransformMatrix(
		secondCube->GetParentTransformAndRotation() * mat4f::translation(3, 0, 3),
		mat4f::rotation(angle * 1.0f, 1.0f, 0.0f, 0.0f),
		mat4f::scaling(0.3f, 0.3f, 0.3f));

	thirdCube->SetTransformMatrix(
		thirdCube->GetParentTransformAndRotation() * mat4f::translation(0.0f, 1.0f, 1.0f),
		mat4f::rotation(-angle * 3.0f, 0.0f, 1.0f, 0.0f),
		mat4f::scaling(0.5f, 0.5f, 0.5f));

	fourthCube->SetTransformMatrix(
		fourthCube->GetParentTransformAndRotation() * mat4f::translation(-5, 0, -5),
		mat4f::rotation(-angle * 2, 0.0f, 1.0f, 0.0f),
		mat4f::scaling(0.3f, 0.3f, 0.3f));

	fifthCube->SetTransformMatrix(
		fifthCube->GetParentTransformAndRotation() * mat4f::translation(1.8f, 0, 1.8f),
		mat4f::rotation(-angle * 0.8f, 0.0f, 1.0f, 0.0f),
		mat4f::scaling(0.6f, 0.6f, 0.6f));

	sixthCube->SetTransformMatrix(
		sixthCube->GetParentTransformAndRotation() * mat4f::translation(1.0f, 0.0f, 1.0f),
		mat4f::rotation(angle * -3, 0.0f, 0.0f, 1.0f),
		mat4f::scaling(0.2f, 0.2f, 0.2f));

#pragma endregion
#pragma region Objects
	// Sponza model-to-world transformation
	Msponza = mat4f::translation(0, -5, 0) *		 // Move down 5 units
		mat4f::rotation(fPI / 2, 0.0f, 1.0f, 0.0f) * // Rotate pi/2 radians (90 degrees) around y
		mat4f::scaling(1);						 // The scene is quite large so scale it down to 5%

	MBush = mat4f::translation(-4, -5, -8) *
		mat4f::rotation(angle * 0, 0.0f, 0.0f, 0.0f) *
		mat4f::scaling(1.0f);

	MCharacter = mat4f::translation(-0, -3, -0) *
		mat4f::rotation(angle * 0.05f, 0.0f, 1.0f, 0.0f) *
		mat4f::scaling(2.0f);

	MTrain = mat4f::translation(-11, 5, -10) *
		mat4f::rotation(1.5f, 0.0f, 1.0f, 0.0f) *
		mat4f::scaling(0.5f);

	MSphere = mat4f::translation(0, 0, -8) *
		mat4f::rotation(angle * 0, 0.0f, 0.0f, 0.0f) *
		mat4f::scaling(1.0f);

	MSkybox = mat4f::translation(0, 0, 0) *
		mat4f::rotation(angle * 0, 0.0f, 0.0f, 0.0f) *
		mat4f::scaling(200);

	gun->SetTransformMatrix(gun->GetCamera()->GetViewToWorldMatrix(), mat4f::translation(1.25f, -2.5f, -4.0f),
		mat4f::rotation(1.2f, 0.0f, 1.0f, 0.0f),
		mat4f::scaling(0.05f));


#pragma endregion

	// Increment the rotation angle.
	angle += angle_vel * dt;

	// Print fps
	fps_cooldown -= dt;
	if (fps_cooldown < 0.0)
	{
		//std::cout << "fps " << (int)(1.0f / dt) << std::endl;
		//		printf("fps %i\n", (int)(1.0f / dt));
		fps_cooldown = 2.0;
	}
}

void OurTestScene::Render()
{
	dxdevice_context->PSSetSamplers(0, 1, &samplerStates[samplerStateIndex]);

	// Bind transformation_buffer to slot b0 of the VS
	dxdevice_context->VSSetConstantBuffers(0, 1, &transformation_buffer);
	// Binds buffers to slots of the PS
	dxdevice_context->PSSetConstantBuffers(0, 1, &camera_and_light_buffer);
	//
	dxdevice_context->PSSetShaderResources(2, 1, &cubeTexture.texture_SRV);

	// Obtain the matrices needed for rendering from the camera
	Mview = camera->Get_WorldToViewMatrix();
	Mproj = camera->get_ProjectionMatrix();

	if (!(camera->GetPosition() == oldCamPosition) || source1->HasMoved() || source2->HasMoved());
	{
		UpdateCameraAndLightBuffer(camera->GetPosition().xyz0(), vec4f(source1->GetPosition(), 0), vec4f(source2->GetPosition(), 0));
		source1->SetOldPositionToPosition();
		source2->SetOldPositionToPosition();
	}


	UpdateCubes(cubes);
	UpdateBullets();

	UpdateTransformationBuffer(Msponza, Mview, Mproj);
	sponza->Render();

	UpdateTransformationBuffer(MBush, Mview, Mproj);
	bush->Render();

	UpdateTransformationBuffer(MCharacter, Mview, Mproj);
	character->Render();

	UpdateTransformationBuffer(MTrain, Mview, Mproj);
	train->Render();

	UpdateTransformationBuffer(gun->GetTransformMatrix(), Mview, Mproj);
	gun->Render();

	UpdateTransformationBuffer(MSphere, Mview, Mproj);
	sphere->Render();

	UpdateTransformationBuffer(MSkybox, Mview, Mproj);
	skybox->Render();

	for (Light* light : lightSources)
	{
		UpdateTransformationBuffer(light->GetTransformMatrix(), Mview, Mproj);
		light->Render();
	}

	oldCamPosition = camera->GetPosition();
}

void OurTestScene::Release()
{
	SAFE_DELETE(sponza);
	SAFE_DELETE(bush);
	SAFE_DELETE(character);
	SAFE_DELETE(train);
	SAFE_DELETE(gun);
	SAFE_DELETE(sphere);
	SAFE_DELETE(camera);

	SAFE_RELEASE(transformation_buffer);
	// + release other CBuffers
	SAFE_RELEASE(camera_and_light_buffer);

	for (ID3D11SamplerState* state : samplerStates)
	{
		SAFE_RELEASE(state);
	}
}

void OurTestScene::WindowResize(
	int window_width,
	int window_height)
{
	if (camera)
		camera->aspect = float(window_width) / window_height;

	Scene::WindowResize(window_width, window_height);
	ToggleClipping(true);
}

void OurTestScene::InitTransformationBuffer()
{
	HRESULT hr;
	D3D11_BUFFER_DESC MatrixBuffer_desc = { 0 };
	MatrixBuffer_desc.Usage = D3D11_USAGE_DYNAMIC;
	MatrixBuffer_desc.ByteWidth = sizeof(TransformationBuffer);
	MatrixBuffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	MatrixBuffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	MatrixBuffer_desc.MiscFlags = 0;
	MatrixBuffer_desc.StructureByteStride = 0;
	ASSERT(hr = dxdevice->CreateBuffer(&MatrixBuffer_desc, nullptr, &transformation_buffer));
}

void OurTestScene::InitCameraAndLightBuffer()
{
	HRESULT hr;
	D3D11_BUFFER_DESC MatrixBuffer_desc = { 0 };
	MatrixBuffer_desc.Usage = D3D11_USAGE_DYNAMIC;
	MatrixBuffer_desc.ByteWidth = sizeof(CameraAndLightBuffer);
	MatrixBuffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	MatrixBuffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	MatrixBuffer_desc.MiscFlags = 0;
	MatrixBuffer_desc.StructureByteStride = 0;
	ASSERT(hr = dxdevice->CreateBuffer(&MatrixBuffer_desc, nullptr, &camera_and_light_buffer));
}

void OurTestScene::UpdateTransformationBuffer(
	mat4f ModelToWorldMatrix,
	mat4f WorldToViewMatrix,
	mat4f ProjectionMatrix)
{
	// Map the resource buffer, obtain a pointer and then write our matrices to it
	D3D11_MAPPED_SUBRESOURCE resource;
	dxdevice_context->Map(transformation_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	TransformationBuffer* matrix_buffer_ = (TransformationBuffer*)resource.pData;
	matrix_buffer_->ModelToWorldMatrix = ModelToWorldMatrix;
	matrix_buffer_->WorldToViewMatrix = WorldToViewMatrix;
	matrix_buffer_->ProjectionMatrix = ProjectionMatrix;
	dxdevice_context->Unmap(transformation_buffer, 0);
}

void OurTestScene::UpdateCameraAndLightBuffer(vec4f CamPosition, vec4f LightPosition1, vec4f LightPosition2)
{
	D3D11_MAPPED_SUBRESOURCE resource;
	dxdevice_context->Map(camera_and_light_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	CameraAndLightBuffer* cameraAndLightBuffer = (CameraAndLightBuffer*)resource.pData;
	cameraAndLightBuffer->CamPosition = CamPosition;

	for (int i = 0; i < sizeof(lightSources) / sizeof(lightSources[0]); i++)
	{
		cameraAndLightBuffer->LightPositions[i] = vec4f(lightSources[i]->GetPosition(), sizeof(lightSources) / sizeof(lightSources[0]));
	}

	dxdevice_context->Unmap(camera_and_light_buffer, 0);
}

void OurTestScene::UpdateCubes(std::vector<Cube*> cubes)
{
	for (Cube* cube : cubes)
	{
		UpdateTransformationBuffer(cube->GetTransformMatrix(), Mview, Mproj);
		cube->Render();
	}
}

void OurTestScene::UpdateBullets()
{
	for (Bullet* bullet : gun->GetBullets())
	{
		if (!bullet->IsActive())
			continue;

		bullet->Update();
		UpdateTransformationBuffer(bullet->GetTransformMatrix(), Mview, Mproj);
		bullet->Render();
	}
}

void OurTestScene::ToggleClipping(bool state)
{
	if (state == false)
	{
		ClipCursor(nullptr);
		ShowCursor(true);
		cursorIsShowing = true;
	}
	else
	{
		ShowCursor(false);
		cursorIsShowing = false;
		ClipCursorToWindow();
	}
}

void OurTestScene::ClipCursorToWindow()
{
	RECT rect;
	GetClientRect(GetActiveWindow(), &rect);

	POINT ul;
	ul.x = rect.left;
	ul.y = rect.top;

	POINT lr;
	lr.x = rect.right;
	lr.y = rect.bottom;

	MapWindowPoints(GetActiveWindow(), nullptr, &ul, 1);
	MapWindowPoints(GetActiveWindow(), nullptr, &lr, 1);

	rect.left = ul.x;
	rect.top = ul.y;

	rect.right = lr.x;
	rect.bottom = lr.y;

	ClipCursor(&rect);
}

void OurTestScene::CreateSamplerState()
{
	HRESULT hr;

	D3D11_SAMPLER_DESC sd =
	{
		D3D11_FILTER_ANISOTROPIC,
		D3D11_TEXTURE_ADDRESS_WRAP,
		D3D11_TEXTURE_ADDRESS_WRAP,
		D3D11_TEXTURE_ADDRESS_WRAP,
		0.0f,
		16,
		D3D11_COMPARISON_NEVER,
		{1.0f, 1.0f, 1.0f, 1.0f },
		-FLT_MAX,
		FLT_MAX,
	};
	ASSERT(hr = dxdevice->CreateSamplerState(&sd, &samplerState));

	samplerStates.push_back(samplerState);

	D3D11_SAMPLER_DESC sd2 =
	{
		D3D11_FILTER_ANISOTROPIC,
		D3D11_TEXTURE_ADDRESS_MIRROR,
		D3D11_TEXTURE_ADDRESS_MIRROR,
		D3D11_TEXTURE_ADDRESS_WRAP,
		0.0f,
		16,
		D3D11_COMPARISON_NEVER,
		{1.0f, 1.0f, 1.0f, 1.0f },
		-FLT_MAX,
		FLT_MAX,
	};
	ASSERT(hr = dxdevice->CreateSamplerState(&sd2, &samplerState2));

	samplerStates.push_back(samplerState2);

	D3D11_SAMPLER_DESC sd3 =
	{
		D3D11_FILTER_ANISOTROPIC,
		D3D11_TEXTURE_ADDRESS_CLAMP,
		D3D11_TEXTURE_ADDRESS_CLAMP,
		D3D11_TEXTURE_ADDRESS_WRAP,
		0.0f,
		16,
		D3D11_COMPARISON_NEVER,
		{1.0f, 1.0f, 1.0f, 1.0f },
		-FLT_MAX,
		FLT_MAX,
	};
	ASSERT(hr = dxdevice->CreateSamplerState(&sd3, &samplerState3));

	samplerStates.push_back(samplerState3);

	D3D11_SAMPLER_DESC sd4 =
	{
		D3D11_FILTER_MIN_MAG_MIP_POINT,
		D3D11_TEXTURE_ADDRESS_WRAP,
		D3D11_TEXTURE_ADDRESS_WRAP,
		D3D11_TEXTURE_ADDRESS_WRAP,
		0.0f,
		16,
		D3D11_COMPARISON_NEVER,
		{1.0f, 1.0f, 1.0f, 1.0f },
		-FLT_MAX,
		FLT_MAX,
	};
	ASSERT(hr = dxdevice->CreateSamplerState(&sd4, &samplerState4));

	samplerStates.push_back(samplerState4);

	D3D11_SAMPLER_DESC sd5 =
	{
		D3D11_FILTER_MIN_MAG_MIP_LINEAR,
		D3D11_TEXTURE_ADDRESS_WRAP,
		D3D11_TEXTURE_ADDRESS_WRAP,
		D3D11_TEXTURE_ADDRESS_WRAP,
		0.0f,
		16,
		D3D11_COMPARISON_NEVER,
		{1.0f, 1.0f, 1.0f, 1.0f },
		-FLT_MAX,
		FLT_MAX,
	};
	ASSERT(hr = dxdevice->CreateSamplerState(&sd5, &samplerState5));

	samplerStates.push_back(samplerState5);

	D3D11_SAMPLER_DESC sd6 =
	{
		D3D11_FILTER_ANISOTROPIC,
		D3D11_TEXTURE_ADDRESS_WRAP,
		D3D11_TEXTURE_ADDRESS_WRAP,
		D3D11_TEXTURE_ADDRESS_WRAP,
		0.0f,
		16,
		D3D11_COMPARISON_NEVER,
		{1.0f, 1.0f, 1.0f, 1.0f },
		-FLT_MAX,
		FLT_MAX,
	};
	ASSERT(hr = dxdevice->CreateSamplerState(&sd6, &samplerState6));

	samplerStates.push_back(samplerState6);
}
