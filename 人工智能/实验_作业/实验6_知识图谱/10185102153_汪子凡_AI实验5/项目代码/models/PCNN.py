# -*- coding: utf-8 -*-

from .BasicModule import BasicModule
import numpy as np
import torch
import torch.nn as nn
import torch.nn.functional as F


class PCNN(BasicModule):
    '''
    the basic model
    Zeng 2014 "Relation Classification via Convolutional Deep Neural Network"
    '''

    def __init__(self, opt):                           #定义网络结构
        super(PCNN, self).__init__()

        self.opt = opt

        self.model_name = 'PCNN'

        self.word_embs = nn.Embedding(self.opt.vocab_size, self.opt.word_dim)    #单词的向量化表示，从已有的 word embedding 中 load 出来
        self.pos1_embs = nn.Embedding(self.opt.pos_size + 1, self.opt.pos_dim)   #与句子中第一个实体的相对位置所对应的向量化表示
        self.pos2_embs = nn.Embedding(self.opt.pos_size + 1, self.opt.pos_dim)   #与句子中第二个实体的相对位置所对应的向量化表示

        feature_dim = self.opt.word_dim + self.opt.pos_dim * 2             #设置每个单词的feature维度

        self.convs = nn.ModuleList(                                        #设置卷积层得到sentence-level feature
            [nn.Conv2d(1, self.opt.filters_num, (k, feature_dim), padding=(int(k / 2), 0)) for k in self.opt.filters])
        all_filter_num = self.opt.filters_num * len(self.opt.filters)
        self.cnn_linear = nn.Linear(all_filter_num, self.opt.sen_feature_dim)

        self.init_word_emb()

        #将lexical-feature 与 sentence-level feature 直接串起来
        self.out_linear = nn.Linear(all_filter_num + self.opt.word_dim * 6, self.opt.rel_num)
        self.dropout = nn.Dropout(self.opt.drop_out)
        self.init_word_emb()
        self.init_model_weight()

    def init_model_weight(self):
        '''
        use xavier to init
        '''
        nn.init.xavier_normal_(self.cnn_linear.weight)
        nn.init.constant_(self.cnn_linear.bias, 0.)
        nn.init.xavier_normal_(self.out_linear.weight)
        nn.init.constant_(self.out_linear.bias, 0.)
        for conv in self.convs:
            nn.init.xavier_normal_(conv.weight)
            nn.init.constant_(conv.bias, 0)


    def init_word_emb(self):
        if self.opt.use_gpu:
            self.word_embs.weight.data.copy_(self.get_word_emb().cuda())
        else:
            self.word_embs.weight.data.copy_(self.get_word_emb())

    def get_word_emb(self):
        return torch.from_numpy(np.load(self.opt.w2v_path))

    def forward(self, x):
        lexical_feature, word_feautre, left_pf, right_pf = x

        #处理输入，得到lexical level feature
        batch_size = lexical_feature.size(0)
        lexical_level_emb = self.word_embs(torch.Tensor.long(lexical_feature))  # (batch_size, 6, word_dim
        lexical_level_emb = lexical_level_emb.view(batch_size, -1)
        # lexical_level_emb = lexical_level_emb.sum(1)

        # 得到sentence level feature
        word_emb = self.word_embs(torch.Tensor.long(word_feautre))
        left_emb = self.pos1_embs(torch.Tensor.long(left_pf))
        right_emb = self.pos2_embs(torch.Tensor.long(right_pf))

        sentence_feature = torch.cat([word_emb, left_emb, right_emb], 2)

        # 后将处理过后的 Sentence Level Feature 与 Lexical Level Feature 拼接
        x = sentence_feature.unsqueeze(1)
        x = self.dropout(x)
        x = [F.relu(conv(x)).squeeze(3) for conv in self.convs]
        x = [F.max_pool1d(i, i.size(2)).squeeze(2) for i in x]
        x = torch.cat(x, 1)


        sen_level_emb = x

        x = torch.cat([lexical_level_emb, sen_level_emb], 1)
        x = self.dropout(x)
        x = self.out_linear(x)

        return x
