/**
 @file Texture.cpp
*/
#define NOMINMAX
#include <GL/glew.h>
#include "Texture.h"
#include<stdint.h>
#include<vector>
#include<fstream>
#include <iostream>
#include <algorithm>

/// �e�N�X�`���֘A�̊֐���N���X���i�[���閼�O���.
namespace Texture
{

/**
* �F�f�[�^���擾����.
*
* @param x  X���W.
* @param y  Y���W.
*
* @return ���W(x, y)�̐F��0.0�`1.0�ŕ\�����l.
*         �F�v�f���f�[�^�ɑ��݂��Ȃ��ꍇ�ARGB��0.0�AA��1.0�ɂȂ�.
*/
glm::vec4 ImageData::GetColor(int x, int y) const
{
	// ���W���摜�͈̔͂ɐ���.
	x = std::max(0, std::min(x, width - 1));
	y = std::max(0, std::min(y, height - 1));

	if (type == GL_UNSIGNED_BYTE) {
    // �e�F�P�o�C�g�̃f�[�^.
	glm::vec4 color(0, 0, 0, 255);
	if (format == GL_BGRA) {
	      // BGRA�̏��ԂłP�o�C�g���A���v�S�o�C�g�i�[����Ă���.
			const uint8_t* p = &data[x * 4 + y * (width * 4)];
			color.b = p[0];
			color.g = p[1];
			color.r = p[2];
			color.a = p[3];

		}else if (format == GL_BGR) {
		      // BGR�̏��ԂłP�o�C�g���A���v�R�o�C�g�i�[����Ă���.
			const uint8_t* p = &data[x * 3 + y * (width * 3)];
			color.b = p[0];
			color.g = p[1];
			color.r = p[2];

		}else if (format == GL_RED) {
		      // �ԐF�����A���v�P�o�C�g�i�[����Ă���.
			color.r = data[x + y * width];
			
		}

		return color / 255.0f;

	}else if (type == GL_UNSIGNED_SHORT_1_5_5_5_REV) {
	    // �F���Q�o�C�g�ɋl�ߍ��܂ꂽ�f�[�^.
		glm::vec4 color(0, 0, 0, 1);
		const uint8_t* p = &data[x * 2 + y * (width * 2)];
		const uint16_t c = p[0] + p[1] * 0x100; // 2�̃o�C�g������.
		if (format == GL_BGRA) {
	      // 16�r�b�g�̃f�[�^����e�F�����o��.
			color.b = static_cast<float>((c & 0b0000'0000'0001'1111));
			color.g = static_cast<float>((c & 0b0000'0011'1110'0000) >> 5);
			color.r = static_cast<float>((c & 0b0111'1100'0000'0000) >> 10);
			color.a = static_cast<float>((c & 0b1000'0000'0000'0000) >> 15);

		}

		return color / glm::vec4(31.0f, 31.0f, 31.0f, 1.0f);

	}

	return glm::vec4(0, 0, 0, 1);
}

	/*
	2D�e�N�X�`�����쐬����.

	@param width   �e�N�X�`���̕�(�s�N�Z����).
	@param height  �e�N�X�`���̍���(�s�N�Z����).
	@param data    �e�N�X�`���f�[�^�ւ̃|�C���^.
	@param format  �]�����摜�̃f�[�^�`��.
	@param type    �]�����摜�̃f�[�^�i�[�`��.
	@retval 0�ȊO  �쐬�����e�N�X�`���E�I�u�W�F�N�g��ID.
	@retval 0      �e�N�X�`���̍쐬�Ɏ��s.
	*/
	GLuint CreateImage2D(GLsizei width, GLsizei height, const GLvoid* data,
		GLuint format,GLenum type)
	{
		GLuint id;
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D,
			0, GL_RGBA8, width, height, 0, format, type, data);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		const GLenum result = glGetError();
		if (result != GL_NO_ERROR) {
			std::cerr << "ERROR: �e�N�X�`���̍쐬�Ɏ��s(0x" << std::hex << result << ").";
			glBindTexture(GL_TEXTURE_2D, 0);
			glDeleteTextures(1, &id);
			return 0;

		}

		//�e�N�X�`���̃p�����[�^��ݒ肷��
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);�ő�@�ʏ�͂�����
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);�ŏ��@�ʏ�͂�����
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);//�ǂ����Ă��͂����肵������
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);//�ǂ����Ă��͂����肵������
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//�����K
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//�����K
		// 1�v�f�̉摜�f�[�^�̏ꍇ�A(R,R,R,1)�Ƃ��ēǂݎ����悤�ɐݒ肷��.
		if(format==GL_RED)
			{
			const GLint swizzle[] = { GL_RED,GL_RED, GL_RED, GL_ONE };
			glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzle);
			}

		glBindTexture(GL_TEXTURE_2D, 0);
		return id;
	}



/*
�t�@�C������2D�e�N�X�`����ǂݍ���.

 @param path 2D�e�N�X�`���Ƃ��ēǂݍ��ރt�@�C����.

 @retval 0�ȊO �쐬�����e�N�X�`���E�I�u�W�F�N�g��ID.
         0     �e�N�X�`���̍쐬�Ɏ��s.
*/
	GLuint LoadImage2D(const char*path)
	{
		ImageData imageData;
		if (!LoadImage2D(path, &imageData)) {
			return 0;
		
		}
		return CreateImage2D(imageData.width, imageData.height, imageData.data.data(),
			imageData.format, imageData.type);

	}

/**
* �t�@�C������摜�f�[�^��ǂݍ���.
*
* @param path      �摜�Ƃ��ēǂݍ��ރt�@�C���̃p�X.
* @param imageData �摜�f�[�^���i�[����\����.
*
* @retval true  �ǂݍ��ݐ���.
* @retval false �ǂݍ��ݎ��s.
*/
bool LoadImage2D(const char* path, ImageData* imageData)
 {

		//TGA�w�b�_��ǂݍ���.
		std::basic_ifstream<uint8_t>ifs;
		ifs.open(path, std::ios_base::binary);

		//�ǂݍ��݂𖾎��I�ɐݒ肵�đ�������
		std::vector<uint8_t>tmp(1024 * 1024);
		ifs.rdbuf()->pubsetbuf(tmp.data(), tmp.size());

		uint8_t tgaHeader[18];
		ifs.read(tgaHeader, 18);

		//�C���[�WID���΂�.
		ifs.ignore(tgaHeader[0]);

		//�J���[�}�b�v���΂�
		if (tgaHeader[1])
		{
			const int colorMapLength = tgaHeader[5] + tgaHeader[6] * 0x100;
			const int colorMapEntrySize = tgaHeader[7];
			const int colorMapSize = colorMapLength * colorMapEntrySize / 8;
			ifs.ignore(colorMapSize);
		}

		// �摜�f�[�^��ǂݍ���.
		const int width = tgaHeader[12] + tgaHeader[13] * 0x100;
		const int height = tgaHeader[14] + tgaHeader[15] * 0x100;
		const int pixelDepth = tgaHeader[16];
		const int imageSize = width * height*pixelDepth / 8;
		std::vector<uint8_t>buf(imageSize);
		ifs.read(buf.data(), imageSize);

		// �摜�f�[�^���u�ォ�牺�v�Ŋi�[����Ă���ꍇ�A�㉺�����ւ���
		if (tgaHeader[17] & 0x20)
		{
			const int lineSize = width * pixelDepth / 8;
			std::vector<uint8_t>tmp(imageSize);
			std::vector<uint8_t>::iterator source=buf.begin();
			std::vector<uint8_t>::iterator destination=tmp.end();
			for (int i = 0; i < height; ++i)
			{
				destination -= lineSize;
				std::copy(source, source + lineSize, destination);
				source += lineSize;
			}
			buf.swap(tmp);
		}

		// �ǂݍ��񂾉摜�f�[�^����e�N�X�`�����쐬����.
		GLenum type = GL_UNSIGNED_BYTE;
		GLenum format = GL_BGRA;
		if (tgaHeader[2] == 3)
		{
			format = GL_RED;
		}
		if (tgaHeader[16] == 24)
		{
			format = GL_BGR;
		}
		else if (tgaHeader[16]==16)
		{
			type = GL_UNSIGNED_SHORT_1_5_5_5_REV;
		}
		imageData->width = width;
		imageData->height = height;
		imageData->format = format;
		imageData->type = type;
		imageData->data.swap(buf);
		return true;

	}

	/**
	* �R���X�g���N�^.
	*
	* @param texId �e�N�X�`���E�I�u�W�F�N�g��ID.
	*/
	Image2D::Image2D(GLuint texId)
		{
		Reset(texId);
		}
	
		/**
		* �f�X�g���N�^.
		*/
		Image2D::~Image2D()
		{
		glDeleteTextures(1, &id);
		}
	
		/**
		* �e�N�X�`���E�I�u�W�F�N�g��ݒ肷��.
		*
		* @param texId �e�N�X�`���E�I�u�W�F�N�g��ID.
		*/
		void Image2D::Reset(GLuint texId)
		{
		glDeleteTextures(1, &id);
		id = texId;
		if(id){
		//�e�N�X�`���̕��ƍ������擾����
			glBindTexture(GL_TEXTURE_2D, id);
			glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
			glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		}
	
		/**
		 * �e�N�X�`���E�I�u�W�F�N�g���ݒ肳��Ă��邩���ׂ�.
		 *
		 * @retval true  �ݒ肳��Ă��Ȃ�.
		 * @retval false �ݒ肳��Ă���.
		 */
		bool Image2D::IsNull() const
		{
		return id==0;
		}
	
		/**
		* �e�N�X�`���E�I�u�W�F�N�g���擾����.
		*
		* @return �e�N�X�`���E�I�u�W�F�N�g��ID.
		*/
		GLuint Image2D::Get() const
		{
		return id;
		}

		/**
		*2D�e�N�X�`�����쐬����
		*/
		Image2DPtr Image2D::Create(const char*path) 
		{

			return std::make_shared<Image2D>(LoadImage2D(path));

		}
	

} // namespace Texture